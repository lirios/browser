
msg() {
    lightblue='\033[1;34m'
    reset='\e[0m'
    echo -e "${lightblue}$@${reset}"
}

travis_start() {
    echo -e "travis_fold:start:$@"
}

travis_end() {
    echo -e "travis_fold:end:$@"
}
