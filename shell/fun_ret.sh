#########################################################################
# File Name: fun_ret.sh
# Desc:
# Author: Andy-wei.hou
# mail: wei.hou@scaleflux.com
# Created Time: Thu Feb  9 14:09:12 2023
# Log: 
#########################################################################
#!/bin/bash
#!/bin/bash
function mytest() {
     
    # echo "mytest function"
    # echo "argv[1] = $1"
 
    if [ $1 = "1" ] ;then
        return 1
    else
        return 256
    fi
}

$(mytest 1)
let ret=$?
echo "return $ret"

$(mytest 2)
ret=$?
echo "return $ret"
