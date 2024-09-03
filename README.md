# snippets-scripts
Scripts and Snippets of code that may be (re)useful

## Terminal Setup
* https://github.com/ohmyzsh/ohmyzsh
* https://github.com/romkatv/powerlevel10k?tab=readme-ov-file

### .anyshrc
```
export LS_OPTS='-GF'
export LSCOLORS=ExFxCxDxBxegedabagacad
alias ls='ls ${LS_OPTS}'
alias ll='ls -lhtr'
alias lla='ls -lhtra'

mkcd ()
{
    mkdir -p "$1"
    cd "$1"
}

alias bzl='mbzl'

bgen ()
{
        gh dbx preflight bzl-gen-preflight
        git status
}

alias bzl_gen='bgen'
alias bzlgen='bgen'

# Adi's Git aliases
alias gbv='git branch -av'
alias gs='git status'
alias gd='git diff'
alias ga='git add'
alias gco='git checkout'
alias gca='git commit --amend'
alias gpr='git pull --rebase'
alias gp='git pull'
alias glop="git log --pretty=format:'%C(yellow)%h| %Cred%ad| %Cblue%aL|%Cgreen%d %Creset%s' --date=short"

eval $(ssh-agent)
ssh-add --apple-use-keychain ~/.ssh/id_ed25519
```

## canvas.wisc grading helper
student submissions (graded) are local file disks
form entry on canvas.wisc
jQuery is imported in the canvas speedgrader tool/page
python code to expose local files via http/https to pull from the browser on canvas via jquery

run python web server to make student grading files available.. https probably necessary

C:\Users\Elcot\AppData\Local\Google\Chrome\Application\chrome.exe --user-data-dir="C:/Chrome dev session" --disable-web-security
chromium browser --user-data-dir="C:/Chrome dev session" --disable-web-security

/opt/google/chrome/google-chrome --profile-directory=Default --user-data-dir="C:/Chrome dev session" --disable-web-security


goto https://localhost:4443 , accept the certificate, verify you can see the files.

use google chrome (or FF), and get the console (easy way is to inspect element and switch tab)
paste the below in the console, and call function..
or just paste the piece of code you want to use.


## randStrings
random string generation, in some thrift client - cpp
Also Command Line parameter parsing


## c_theads_CVs
Threads and condition variables in c
Simple Producer consumer


## pdsh sample
Parallel Shell 


## gitignore
rename to .gitignore in relevant folder
