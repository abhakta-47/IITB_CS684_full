#!/bin/bash
set -eE -o functrace
failure() {
  local lineno=$1
  local msg=$2
  echo "Failed at $lineno: $msg"
}
trap 'failure ${LINENO} "$BASH_COMMAND"' ERR

# opam switch 4.03.0 > /dev/null
eval $(opam env)
# default values
node='display'
path='node.ept'
gui=true
limit=""
prompt="Specify node with 's', path of the heptagon file with 'p' "
prompt+="and optionally -c 1 to run in non-gui mode.\nDefault is -s "
prompt+="display -p node.ept\n"
prompt+="limit for cli -l <number>\n"
while getopts 's:p:k:c:l:h' flag
do
    case "${flag}" in
        s) node=${OPTARG} ;;
        p) path=${OPTARG} ;;
        k) compile_only=${OPTARG} ;;
        c) gui=false ;;
        l) limit=${OPTARG} ;;
        h) printf "$prompt" ; exit 0 ;;
    esac
done

heptc -target c -s $node -hepts $path 
filename="${path##*/}";
filename="${filename%.*}"
filename="${filename,}"
if [[ -z "${filename}" ]]
then echo "Please enter a valid filepath!"; exit 0;
fi

cd $filename"_c/"
gcc -w -c -I $(heptc -where)/c *.c > /dev/null
# gcc -Wall -c -I $(heptc -where)/c _main.c  
gcc -w -o $node *.o > /dev/null
# echo "${node}"
# echo "${filename^}"
if [ ! -z "${compile_only}" ] ; then
  exit 0
fi

if [ "$gui" = true ] ; then
  cd ..
  hepts -mod "${filename^}" -node $node -exec $filename"_c/"$node
else
  echo "Executing program in non-gui mode, enter inputs and press enter:"
  ./"$node" ${limit:-""}
fi