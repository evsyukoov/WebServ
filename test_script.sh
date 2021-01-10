

curl -X PUT -H "Content-Language: en, fr, a-ed-1871" -H "Content-Type: application/x-www-form-urlencoded; charset=iso-1234 " --data "some_data"  127.0.0.1:1234///lang//put.txt
#curl -H "Accept-Language:  fr, da-fd;q=0.2" -H "Accept-Charset:  UTF-8;q=0.4, iso-1234;q=0.2"  127.0.0.1:1234/lang//put.txt
curl -H "Accept-Language:  fr, da-fd;q=0.2" -H "Accept-Charset: UTF-8;q=0.4, iso-1234;q=0.2" 127.0.0.1:1234/lang//put.txt


