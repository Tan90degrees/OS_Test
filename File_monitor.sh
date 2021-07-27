clear
printf "Input file name: "
read filename
flag="`ls -l $filename|cut -b 1`"
if [ "$flag" != "-" ];
then
    echo -e "Can't find the file [$filename]!"
    exit
fi
echo -e "Current status of [$filename] is:\n`ls -l $filename`"
start="`ls -l $filename|awk '{print $5}'`"
now="$start"
num=0
for i in 1 2 3 4 5 6 7 8 9 10;
do
    after="`ls -l $filename|awk '{print $5}'`"
    if [ "$now" == "$after" ];
    then
        echo "test file's status ..."
    else
        echo "file [$filename] size changed!"
        now="$after"
        num="`expr $num + 1`"
    fi
    if [ "$num" == "2" ];
    then
        echo "Change number exceed two, test end!"
        exit
    fi
    sleep 5
done
echo "test number exceed ten!"
        
    