if [ "$#" != "1" ];
then
    echo "Usage: usr_monitor username"
    exit
fi
users="`who|awk '{print $1}'`"
echo "Current user list is:"
echo "$users"
echo
flag="`echo "$users"|awk '!x[$0]++'`"
echo "user list without repeat:"
echo "$flag"
echo
get="`echo "$flag"|grep -e "$1"`"
if [ "$get" == "" ];
then
    while [ "$get" == "" ];
    do
        users="`who|awk '{print $1}'`"
        flag="`echo "$users"|awk '!x[$0]++'`"
        get="`echo "$flag"|grep -e "$1"`"
        printf "\nwaiting user [$1] ..."
        sleep 5
    done
    echo
    echo "user [$1] is logon"
    exit
else
    echo "user [$1] is logon"
    exit
fi