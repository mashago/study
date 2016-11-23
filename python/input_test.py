sentence=raw_input('sentence: ')
s=80
t=len(sentence)
l=(s-t-2)//2

aaa = 123
bbb = 123
aaa = aaa / 3
bbb = bbb // 3
print "aaa = ", aaa
print "bbb = ", bbb

print()
print(' '*l+'+'+'-'*(t+2)+'+')
print(' '*l+'|'+' '*(t+2)+'|')
print(' '*l+'|'+' '+sentence+' '+'|')
print(' '*l+'|'+' '*(t+2)+'|')
print(' '*l+'+'+'-'*(t+2)+'+')
print()

