#!/bin/bash
i=0
while read -r file
do
        if [ ! -f $file ]; then
                continue
        fi
        while [ -f `printf "event%03d.root" $((i))` ]
        do
                echo -n
                printf ">>event%03d.root\n" $((++i))
        done
        #cp $file "event$i.root"
        printf "event%03d.root\n" $((i))
        mv $file `printf "event%03d.root\n" $((i))`
        [ $((++i)) ]
done
