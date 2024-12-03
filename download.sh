FILE=./test/gist.tar.gz

#PACKAGE_NAME="curl"

#if dbkg -l | grep -q "$PACKAGE_NAME";then
#    echo "$PACKAGE_NAME is already installed"
#fi
#else
    sudo apt-get install -y curl
#fi
mkdir test
if [ ! -f "./test/gist.tar.gz" ]; then
    curl ftp://ftp.irisa.fr/local/texmex/corpus/gist.tar.gz --output ./test/gist.tar.gz -C -
fi


tar -zxvf ./test/gist.tar.gz -C ./test
