# Minishell 🐚

A simple shell implementation written in C, built as part of the **42 School curriculum**.  
The project replicates essential features of `bash`, handling commands, pipes, redirections, signals, and environment variables.

---

## ✨ Features

- Prompt display waiting for user input.
- Command execution with arguments (support for absolute/relative paths and `$PATH` search).
- Builtins:
  - `echo` (with `-n` option)
  - `cd` (with relative/absolute paths and `-`)
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`
- Environment variable handling (`$VAR`, `$?`, etc.).
- Quotes handling (`'single'`, `"double"`).
- Pipes (`|`) and redirections (`<`, `>`, `>>`, `<<` heredoc).
- Signal handling:
  - `ctrl-C` (interrupts current command / heredoc)
  - `ctrl-D` (EOF → exits shell)
  - `ctrl-\` (ignored like bash).
- Exit status codes follow Bash’s behavior.

---

## 🛠️ Installation

Clone the repository and build:

```bash
git clone https://github.com/your-username/minishell.git
cd minishell
make

