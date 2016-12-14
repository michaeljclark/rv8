//
//  node.h
//

#ifndef riscv_node_h
#define riscv_node_h

namespace riscv {

	/*
	 * work-in-progress node class for eventual SMP implementation
	 *
	 * TODO
	 *
	 *  - change rv-sys to create a node with n harts
	 *  - move the shared memory map into the node class
	 *  - rewire debug CLI to node and allow selection of hart
	 */

	template <typename P>
	struct node_processor : P, std::thread
	{
		node_processor() : P(), std::thread(&node_processor::mainloop) {}

		void mainloop()
		{
			P::init();
			P::run();
		}
	};

	struct node_fault
	{
		static node_fault *current;
	};

	node_fault* node_fault::current = nullptr;

	template <typename P>
	struct node : node_fault
	{
		std::vector<std::shared_ptr<P>> nodes;

		node(size_t num_nodes)
		{
			init();
			for (size_t i = 0; i < num_nodes; i++) {
				nodes.push_back(std::make_shared<P>());
			}
		}

		static void signal_handler(int signum, siginfo_t *info, void *)
		{
			static_cast<node<P>*>
				(node_fault::current)->signal_dispatch(signum, info);
		}

		void signal_dispatch(int signum, siginfo_t *info)
		{
			printf("SIGNAL   :%s\n", signal_name(signum));
		}

		void init()
		{
			/*
			 * Note: the processor currently runs in the main thread.
			 * When SMP is implemented, hart threads will be changed
			 * to block all interrupts except for SIGSEGV and the main
			 * thread will be configured to monitor for interrupts and
			 * signals to deliver to the hart threads.
			 */

			// block signals before so we don't deadlock in signal handlers
			sigset_t set;
			sigemptyset(&set);
			sigaddset(&set, SIGSEGV);
			sigaddset(&set, SIGTERM);
			sigaddset(&set, SIGQUIT);
			sigaddset(&set, SIGINT);
			sigaddset(&set, SIGHUP);
			sigaddset(&set, SIGUSR1);
			if (pthread_sigmask(SIG_BLOCK, &set, NULL) != 0) {
				panic("can't set thread signal mask: %s", strerror(errno));
			}

			// disable unwanted signals
			sigset_t sigpipe_set;
			sigemptyset(&sigpipe_set);
			sigaddset(&sigpipe_set, SIGPIPE);
			sigprocmask(SIG_BLOCK, &sigpipe_set, nullptr);

			// install signal handler
			struct sigaction sigaction_handler;
			memset(&sigaction_handler, 0, sizeof(sigaction_handler));
			sigaction_handler.sa_sigaction = &node<P>::signal_handler;
			sigaction_handler.sa_flags = SA_SIGINFO;
			sigaction(SIGSEGV, &sigaction_handler, nullptr);
			sigaction(SIGTERM, &sigaction_handler, nullptr);
			sigaction(SIGQUIT, &sigaction_handler, nullptr);
			sigaction(SIGINT, &sigaction_handler, nullptr);
			sigaction(SIGHUP, &sigaction_handler, nullptr);
			sigaction(SIGUSR1, &sigaction_handler, nullptr);
			node_fault::current = this;

			/* unblock signals */
			if (pthread_sigmask(SIG_UNBLOCK, &set, NULL) != 0) {
				panic("can't set thread signal mask: %s", strerror(errno));
			}
		}

		void shutdown()
		{
			for (auto &node : nodes) {
				node->shutdown();
				node->join();
			}
		}
	};

}

#endif
