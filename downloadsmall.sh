FILE=./sift/gist.tar.gz

#PACKAGE_NAME="curl"

#if dbkg -l | grep -q "$PACKAGE_NAME";then
#    echo "$PACKAGE_NAME is already installed"
#fi
#else
    sudo apt-get install -y curl
#fi
mkdir test
if [ ! -f "./test/sift.tar.gz" ]; then
    curl ftp://ftp.irisa.fr/local/texmex/corpus/siftsmall.tar.gz --output ./test/sift.tar.gz -C -
fi


tar -zxvf ./test/sift.tar.gz -C ./test
