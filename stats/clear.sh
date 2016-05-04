#!/bin/sh

echo "client,id,subs,pubs,wrong,displayed,reschedlued,skipped" > data_client.csv
echo "broker,id,received_subs,sent_subs,received_pubs,sent_pubs" > data_broker.csv

sed -e 's/INFO\ (.*)Network.*: //' data.csv | sed '/^$/d' > data_clear.csv
cat data_clear.csv | grep c >> data_client.csv
cat data_clear.csv | grep b >> data_broker.csv
