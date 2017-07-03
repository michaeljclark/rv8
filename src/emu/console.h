//
//  console.h
//

#ifndef rv_console_h
#define rv_console_h

namespace riscv {

	/* Console Thread */

	template <typename P>
	struct console_device
	{
		P &proc;
		struct termios old_tio, new_tio;
		int pipefds[2];
		std::vector<struct pollfd> pollfds;
		queue_atomic<char> queue;
		volatile bool running;
		volatile bool suspended;
		std::thread thread;

		console_device(P &proc) :
			proc(proc),
			pipefds{0},
			pollfds(),
			queue(1024),
			running(true),
			suspended(false),
			thread(&console_device::mainloop, this)
		{}

		~console_device()
		{
			shutdown();
		}

		void process_output()
		{
			char buf[256];
			ssize_t ret;

			if (pollfds[0].revents & POLLIN) {
				if ((ret = read(pipefds[0], buf, (sizeof(buf)))) < 0) {
					debug("console: socket: read: %s", strerror(errno));
				} else if (write(STDIN_FILENO, buf, ret) < 0) {
					debug("console: socket: write: %s", strerror(errno));
				}
			}
		}

		void process_input()
		{
			char buf[256];
			ssize_t ret;

			if (pollfds[1].revents & POLLIN) {
				if ((ret = read(STDIN_FILENO, buf, (sizeof(buf)))) < 0) {
					debug("console: stdin: read: %s", strerror(errno));
				} else {
					proc.intr_mutex.lock();
					for (ssize_t i = 0; i < ret; i++) {
						queue.push_back(buf[i]);
					}
					proc.intr_cond.notify_one();
					proc.intr_mutex.unlock();
				}
			}
		}

		void mainloop()
		{
			block_signals();
			open_pipe();
			configure_console();
			while (running) {
				pollfds.resize(2);
				pollfds[0].fd = pipefds[0];
				pollfds[0].events = POLLIN;
				pollfds[0].revents = 0;
				pollfds[1].fd = STDIN_FILENO;
				pollfds[1].events = POLLIN;
				pollfds[1].revents = 0;
				if (poll(pollfds.data(), pollfds.size(), -1) < 0 && errno != EINTR) {
					panic("console poll failed: %s", strerror(errno));
				}
				process_output();
				if (!running) break;
				if (suspended) continue;
				process_input();
			}
			restore_console();
			close_pipe();
		}

		void block_signals()
		{
			// block signals
			sigset_t set;
			sigemptyset(&set);
			sigaddset(&set, SIGSEGV);
			sigaddset(&set, SIGTERM);
			sigaddset(&set, SIGQUIT);
			sigaddset(&set, SIGINT);
			sigaddset(&set, SIGHUP);
			sigaddset(&set, SIGUSR1);
			if (pthread_sigmask(SIG_BLOCK, &set, NULL) != 0)
			{
				panic("console_device: can't set thread signal mask: %s",
					strerror(errno));
			}
		}

		void open_pipe()
		{
			/* create socketpair */
			if (pipe(pipefds) < 0) {
				panic("pipe failed: %s", strerror(errno));
			}
			if (fcntl(pipefds[0], F_SETFD, FD_CLOEXEC) < 0) {
				panic("console fcntl(F_SETFD, FD_CLOEXEC) failed: %s", strerror(errno));
			}
			if (fcntl(pipefds[0], F_SETFL, O_NONBLOCK) < 0) {
				panic("console fcntl(F_SETFL, O_NONBLOCK) failed: %s", strerror(errno));
			}
			if (fcntl(pipefds[1], F_SETFD, FD_CLOEXEC) < 0) {
				panic("console fcntl(F_SETFD, FD_CLOEXEC) failed: %s", strerror(errno));
			}
			if (fcntl(pipefds[1], F_SETFL, O_NONBLOCK) < 0) {
				panic("console fcntl(F_SETFL, O_NONBLOCK) failed: %s", strerror(errno));
			}
		}

		void close_pipe() {
			close(pipefds[0]);
			close(pipefds[1]);
		}

		/* setup console */
		void configure_console()
		{
			tcgetattr(STDIN_FILENO, &old_tio);
			new_tio = old_tio;
			new_tio.c_lflag &=(~ICANON & ~ECHO);
			tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);
		}

		/* teardown console */
		void restore_console()
		{
			/* restore settings */
			tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
		}

		/* suspend console input */
		void suspend()
		{
			restore_console();
			suspended = true;
		}

		/* resume console input */
		void resume()
		{
			configure_console();
			suspended = false;
		}

		/* shutdown console thread */
		void shutdown()
		{
			/* set running flag to false and write a null byte to the FIFO */
			running = false;
			unsigned char c = 0;
			/* wake up the console thread so it checks running and exits */
			if (write(pipefds[1], &c, 1) < 0) {
				debug("console: socket: write: %s", strerror(errno));
			}
			/* wait for the console thread to finish */
			thread.join();
		}

		/* check if data is available */
		bool has_char()
		{
			return queue.size() > 0;
		}

		/* read one character */
		u8 read_char()
		{
			return queue.size() > 0 ? queue.pop_front() : 0;
		}

		/* write one character */
		void write_char(u8 c)
		{
			if (write(pipefds[1], &c, 1) < 0) {
				debug("console: socket: write: %s", strerror(errno));
			}
		}
	};

}

#endif