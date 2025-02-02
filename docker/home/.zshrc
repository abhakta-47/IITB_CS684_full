HISTFILE=~/.zsh_history
HISTSIZE=1000
SAVEHIST=1000
setopt INC_APPEND_HISTORY
setopt SHARE_HISTORY
source ~/.zsh/zsh-autosuggestions/zsh-autosuggestions.zsh
source ~/.zsh/zsh-syntax-highlighting/zsh-syntax-highlighting.zsh

PROMPT='%~ > '

# BEGIN opam configuration
# This is useful if you're using opam as it adds:
#   - the correct directories to the PATH
#   - auto-completion for the opam binary
# This section can be safely removed at any time if needed.
[[ ! -r '/home/user/.opam/opam-init/init.zsh' ]] || source '/home/user/.opam/opam-init/init.zsh' > /dev/null 2> /dev/null
eval $(opam env --switch=4.03.0)
# END opam configuration
