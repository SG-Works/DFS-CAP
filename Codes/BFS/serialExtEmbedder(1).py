import sys
import os
NoEmbedPatterns = 1
SizeInputPatterns = 3
M = 25 
T = 1.0
rholist = [0.00]
peelist = [0.05]
etalist = [0.5]
runs = len(rholist) * len(peelist) * len(etalist)
i = 1
exe = "./serialExtensionEmbedder/bin/serialExtEmbedder "
for rho in rholist:
  for eta in etalist:
    for pee in peelist:
      outputFile = "stream-%.3f-%.3f-%.3f-%d-%de3-%d-%d-node" % (rho,pee,eta,M,T,NoEmbedPatterns,SizeInputPatterns)
      generate = "%s %.3f %.3f %.3f %d %f" % (exe, rho, pee, eta, M, T)
      move = "mv stream.txt ./datasets/ChainEpisodes/%s" % (outputFile)
      os.system(generate)
      os.system(move)
      print "RUN ",i," of ",runs," ...DONE\n\n"
      i= i + 1
