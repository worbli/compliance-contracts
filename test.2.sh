#!/bin/sh

worbli system newaccount worbli.admin --transfer worbli.reg EOS7Twda1EMGC1FYH2o9d7APfmwZAMtN3e69wQg1CPjBqQSaWH39P --stake-net "5.0000 WBI" --stake-cpu "10.0000 WBI" --buy-ram-kbytes 300
worbli set contract worbli.reg /home/rob/git/compliance-contracts-cmake/build/contracts/worbli.reg -p worbli.reg

worbli system newaccount worbli.admin --transfer worbli.prov EOS7Twda1EMGC1FYH2o9d7APfmwZAMtN3e69wQg1CPjBqQSaWH39P --stake-net "5.0000 WBI" --stake-cpu "10.0000 WBI" --buy-ram-kbytes 200
worbli set contract worbli.prov /home/rob/git/compliance-contracts-cmake/build/contracts/worbli.prov -p worbli.prov

worbli system newaccount worbli.admin --transfer tokenoro.prov EOS7Twda1EMGC1FYH2o9d7APfmwZAMtN3e69wQg1CPjBqQSaWH39P --stake-net "5.0000 WBI" --stake-cpu "10.0000 WBI" --buy-ram-kbytes 200
worbli set contract tokenoro.prov /home/rob/git/compliance-contracts-cmake/build/contracts/worbli.prov -p tokenoro.prov

worbli system newaccount worbli.admin --transfer client EOS7Twda1EMGC1FYH2o9d7APfmwZAMtN3e69wQg1CPjBqQSaWH39P --stake-net "5.0000 WBI" --stake-cpu "10.0000 WBI" --buy-ram-kbytes 200
worbli set contract client /home/rob/git/compliance-contracts-cmake/build/contracts/client -p client

worbli system newaccount worbli.admin --transfer alice EOS7Twda1EMGC1FYH2o9d7APfmwZAMtN3e69wQg1CPjBqQSaWH39P --stake-net "5.0000 WBI" --stake-cpu "10.0000 WBI" --buy-ram-kbytes 200

worbli system newaccount worbli.admin --transfer bob EOS7Twda1EMGC1FYH2o9d7APfmwZAMtN3e69wQg1CPjBqQSaWH39P --stake-net "5.0000 WBI" --stake-cpu "10.0000 WBI" --buy-ram-kbytes 200


worbli push action worbli.reg addcred '["identity", "identity verified"]' -p worbli.reg
worbli push action worbli.reg addcred '["accredited", "accredited investor"]' -p worbli.reg

worbli push action worbli.reg addprovider '["worbli.prov", "Worbli Foundation"]' -p worbli.reg
worbli push action worbli.reg addprovider '["tokenoro.prov", "Tokenoro"]' -p worbli.reg

worbli push action worbli.reg addprovcred '["worbli.prov", "identity"]' -p worbli.reg
worbli push action worbli.reg addprovcred '["tokenoro.prov", "accredited"]' -p worbli.reg

worbli push action worbli.prov addentry '["alice", "kyc", "true"]' -p worbli.prov
worbli push action worbli.prov addentry '["alice", "accredited", "true"]' -p worbli.prov

worbli push action worbli.prov addentry '["bob", "kyc", "true"]' -p worbli.prov
worbli push action worbli.prov addentry '["bob", "accredited", "true"]' -p worbli.prov

worbli push action client test1 '["alice"]' -p client



