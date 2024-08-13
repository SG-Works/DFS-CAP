datalen = 10000
size = 6
peelist = [0.02,0.03,0.05]
etalist = [0.2,0.4,0.6]

for pee in peelist:
  for eta in etalist:
    meanfreq = datalen / ( (size-1)/(1-eta) + (1/pee) )
    print pee,eta,meanfreq

