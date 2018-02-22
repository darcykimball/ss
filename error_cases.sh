#/bin/bash -e


#
# This script shows some error cases; other cases are covered in screenshots/
#

# (Assuming you already ran:)
# $ ./server -document_root resources/ -port 8888 &


cat req/bad.req | nc localhost 8888
cat req/bad2.req | nc localhost 8888
 

