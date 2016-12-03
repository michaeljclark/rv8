//
//  riscv-debug-cli.h
//

#ifndef riscv_debug_cli_h
#define riscv_debug_cli_h

namespace riscv {

	template<typename P>
	struct debug_cli
	{
		struct cmd_def;
		struct cmd_state;
		typedef std::vector<std::string> args_t;
		typedef std::function<bool(cmd_state&,args_t&)> cmd_fn;
		typedef std::map<std::string,cmd_def> cmd_map;

		struct cmd_def
		{
			cmd_fn fn;
			std::string name;
			std::string description;
		};

		struct cmd_state
		{
			P *proc;
			debug_cli *cli;
		};

		EditLine *el;
		History *hist;
		Tokenizer *tok;
		HistEvent ev;
		cmd_map map;

		static const char* prompt(EditLine *el)
		{
			static char prompt_buf[32] = {};
			cmd_state *st;
			el_get(el, EL_CLIENTDATA, &st);
			snprintf(prompt_buf, sizeof(prompt_buf), "(%s) ", st->proc->name());
			return prompt_buf;
		}

		debug_cli() :
			el(nullptr),
			hist(nullptr),
			tok(nullptr)
		{
			/* set up editline */
			hist = history_init();
			history(hist, &ev, H_SETSIZE, 100);
			tok  = tok_init(NULL);
			el = el_init("rv-cli", stdin, stdout, stderr);
			el_set(el, EL_EDITOR, "emacs");
			el_set(el, EL_PROMPT_ESC, prompt, '\1');
			el_set(el, EL_HIST, history, hist);

			/* add commands to map */
			add_command(cmd_help, "help", "Show help");
			add_command(cmd_reg, "reg", "Show registers");
			add_command(cmd_run, "run", "Resume execution");
			add_command(cmd_quit, "quit", "End the simulation");
		}

		void add_command(cmd_fn fn, std::string name, std::string description)
		{
			map[name] = cmd_def{fn, name, description};
		}

		void help()
		{
			printf("helpxx\n");
			for (auto &ent : map) {
				printf("%-20s %s\n",
					ent.second.name.c_str(),
					ent.second.description.c_str());
			}
		}

		static bool cmd_help(cmd_state &st, args_t &args)
		{
			printf("helpx\n");
			st.cli->help();
			return false;
		}

		static bool cmd_run(cmd_state &, args_t &)
		{
			return true;
		}

		static bool cmd_reg(cmd_state &st, args_t &)
		{
			st.proc->print_csr_registers();
			st.proc->print_int_registers();
			return false;
		}

		[[noreturn]] static bool cmd_quit(cmd_state&, args_t &)
		{
			exit(1);
		}

		void run(P *proc)
		{
			cmd_state st{ proc, this };
			const char *buf = nullptr;
			int num;

			el_set(el, EL_CLIENTDATA, &st);
			proc->debug_enter();
			while ((buf = el_gets(el, &num)) != NULL && num != 0) {
				auto line = ltrim(rtrim(buf));
				auto args = split(line, " ", false, false);
				if (args.size() == 0) continue;
				auto it = map.find(args[0]);
				if (it == map.end()) {
					printf("unknown command %s", args[0].c_str());
					continue;
				}
				if (it->second.fn(st, args)) break;
			}
			proc->debug_leave();
		}
	};
}

#endif
