import requests
import json

url = 'http://solar-log/getjp'
myobj = '{"801":{"170":null}}'

r = requests.post(url, data = myobj)

print(r.text)
#curl -X POST -H 'Content-Type: application/json' -d '{"801":{"170":null}}' http://solar-log/getjp
#{"801":{"170":{"100":"10.06.20 21:39:15",

#loaded_json = json.loads(json.loads(json.loads(r.text)["801"])["170"])

loaded_json = json.loads(r.text)["801"]
print typeof(loaded_json)

loaded_json = json.loads(json.loads(r.text)["801"])

for x in loaded_json:
  print("%s: %s" % (x, loaded_json[x]))
