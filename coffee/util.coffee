binaryToHex = (s) ->
  ret = ""
  i = s.length - 1
  while i >= 3
    part = s.substr(i + 1 - 4, 4)
    accum = 0
    k = 0
    while k < 4
      return valid: false  if part[k] isnt "0" and part[k] isnt "1"
      accum = accum * 2 + parseInt(part[k], 10)
      k++
    if accum >= 10
      ret = String.fromCharCode(accum - 10 + "A".charCodeAt(0)) + ret
    else
      ret = String(accum) + ret
    i -= 4
  if i >= 0
    accum = 0
    k = 0
    while k <= i
      return null  if s[k] isnt "0" and s[k] isnt "1"
      accum = accum * 2 + parseInt(s[k], 10)
      k++
    ret = String(accum) + ret
  ret
hexToBinary = (s) ->
  i = undefined
  k = undefined
  part = undefined
  ret = ""
  lookupTable =
    0: "0000"
    1: "0001"
    2: "0010"
    3: "0011"
    4: "0100"
    5: "0101"
    6: "0110"
    7: "0111"
    8: "1000"
    9: "1001"
    a: "1010"
    b: "1011"
    c: "1100"
    d: "1101"
    e: "1110"
    f: "1111"
  i = 0
  while i < s.length
    if lookupTable.hasOwnProperty(s[i].toLowerCase())
      ret += lookupTable[s[i].toLowerCase()]
    else
      return null
    i++
  ret

percent = (v, o) -> v * (o / 100)

