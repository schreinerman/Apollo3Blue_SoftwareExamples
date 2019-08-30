#!/bin/sh
cd ..
for mcu_template in apollo*template*
do
  version=`cat $mcu_template/version.txt`
  zip -9r ${mcu_template}-v${version}.zip $mcu_template
done

for example in `echo  apollo* | tr " " "\n" | grep -v "template"`
do
  version=`cat $example/version.txt`
  zip -9r ${example}-v${version}.zip $example
done

for example in `echo  ama* | tr " " "\n" | grep -v "template"`
do
  version=`cat $example/version.txt`
  zip -9r ${example}-v${version}.zip $example
done
