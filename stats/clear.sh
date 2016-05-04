#!/bin/sh
sed -e 's/INFO\ (.*)Network.*: //' data.csv | sed '/^$/d' > data_clear.csv
