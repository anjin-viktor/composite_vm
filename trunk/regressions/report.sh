#!/bin/sh


COMP=../src/composite_vm
OUT=Results.txt

rm -f $OUT

for i in *.mpr; do
    echo "Result for $i:" >> $OUT

    if [ `echo $i | cut -c 1,2,3,4 ` = "pos_" ]; then
        $COMP $i $i.Res >> $OUT
        line=`diff -q $i.Res $i.Nrm`

        if [ ! -z "$line" ]; then
            echo "Outfile for $i should be different!"
            exit 1
        fi
    else
    	$COMP $i /dev/null >> $OUT
    fi
    echo "" >> $OUT
done

line=`diff -q Results.txt Normal.txt`
if [ ! -z "$line" ]; then
    echo "Test(s) FAILED!!!"
    echo "Something broken or out of sync! Check out Broken.diff"
    diff -u Normal.txt Results.txt > Broken.diff
    exit 1
else
    echo "Tests PASSED FINE"
    exit 0
fi
