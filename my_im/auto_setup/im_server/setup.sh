
INSTALL_DIR=.
IM_SERVER=own-im-server-*

FILE_SERVER=file_server
LOGIN_SERVER=login_server
MSG_SERVER=msg_server
ROUTE_SERVER=route_server
MSFS_SERVER=msfs
HTTP_MSG_SERVER=http_msg_server
DB_PROXY_SERVER=db_proxy_server


FILE_SERVER_CONF=fileserver.conf
LOGIN_SERVER_CONF=loginserver.conf
MSG_SERVER_CONF=msgserver.conf
ROUTE_SERVER_CONF=routeserver.conf
MSFS_SERVER_CONF=msfs.conf
HTTP_MSG_SERVER_CONF=httpmsgserver.conf
DB_PROXY_SERVER_CONF=dbproxyserver.conf

print_hello(){
    echo "==========================================="
    echo "$1 im server for TeamTalk"
    echo "==========================================="
}

check_user() {
    if [ $(id -u) != "0" ]; then
        echo "Error: You must be root to run this script, please use root to install im_c++_server"
        exit 1
    fi
}

build_im_server() {

    #yum -y install yum-fastestmirror
    #clean_yum
    #yum -y install libuuid-devel
    #yum -y install apr-util-devel
    #mkdir -p $INSTALL_DIR
    #tar zxvf $IM_SERVER.tar.gz -C $INSTALL_DIR/
    if [ $? -eq 0 ]; then
        echo "unzip im-server successed."
        set -x
        cp -f ./conf/$LOGIN_SERVER_CONF $INSTALL_DIR/$IM_SERVER/$LOGIN_SERVER/
        cp -f ./conf/$MSG_SERVER_CONF $INSTALL_DIR/$IM_SERVER/$MSG_SERVER/
        cp -f ./conf/$ROUTE_SERVER_CONF $INSTALL_DIR/$IM_SERVER/$ROUTE_SERVER/
        cp -f ./conf/$FILE_SERVER_CONF $INSTALL_DIR/$IM_SERVER/$FILE_SERVER/
        cp -f ./conf/$MSFS_SERVER_CONF $INSTALL_DIR/$IM_SERVER/$MSFS_SERVER/
        cp -f ./conf/$HTTP_MSG_SERVER_CONF $INSTALL_DIR/$IM_SERVER/$HTTP_MSG_SERVER/
        cp -f ./conf/$DB_PROXY_SERVER_CONF $INSTALL_DIR/$IM_SERVER/$DB_PROXY_SERVER/

        cd $IM_SERVER
        chmod +x ./sync_lib_for_zip.sh
        ./sync_lib_for_zip.sh
        cd ..
        #cp -f ./$IM_SERVER/lib/* $INSTALL_DIR/$IM_SERVER/$LOGIN_SERVER/
        #cp -f ./$IM_SERVER/lib/* $INSTALL_DIR/$IM_SERVER/$MSG_SERVER/
        #cp -f ./$IM_SERVER/lib/* $INSTALL_DIR/$IM_SERVER/$ROUTE_SERVER/
        #cp -f ./$IM_SERVER/lib/* $INSTALL_DIR/$IM_SERVER/$FILE_SERVER/
        #cp -f ./$IM_SERVER/lib/* $INSTALL_DIR/$IM_SERVER/$MSFS_SERVER/
        #cp -f ./$IM_SERVER/lib/* $INSTALL_DIR/$IM_SERVER/$HTTP_MSG_SERVER/
        #cp -f ./$IM_SERVER/lib/* $INSTALL_DIR/$IM_SERVER/$DB_PROXY_SERVER/
        chmod 755 $INSTALL_DIR/$IM_SERVER/restart.sh

        set +x
    else
        echo "Error: unzip im-server failed."
        return 1
    fi
}

run_im_server() {
    cd $INSTALL_DIR/$IM_SERVER
    ./restart.sh $LOGIN_SERVER
    ./restart.sh $ROUTE_SERVER
    ./restart.sh $MSG_SERVER
    ./restart.sh $FILE_SERVER
    ./restart.sh $MSFS_SERVER
    ./restart.sh $HTTP_MSG_SERVER
    ./restart.sh $DB_PROXY_SERVER
}
print_help() {
    echo "Usage: "
    echo "  $0 check --- check environment"
    echo "  $0 install --- check & run scripts to install"
}
case $1 in
    install)
        print_hello $1
        check_user
    build_im_server
        run_im_server
        ;;
    *)
        print_help
        ;;
esac
