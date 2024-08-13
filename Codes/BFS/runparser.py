import sys,os
#for each size we create a file. Each directory in parser will be of the form eta-rho-p-M-T-N_i
rholist = [0.00]
peelist = [0.05]
etalist = [0.5]
Tlist = [50.0] #the no. of time ticks is T*1000
NoEmbedPatternslist = [1]
SizeInputPatterns = 3
Mlist = [25]

freqlist = [0]
entlist = [0]
explist = [10.0]
miner = "./n-AutoMineGraphs-feb22/bin/n-graph-miner"

for rho in rholist:
  for pee in peelist:
    for eta in etalist:
      for NoEmbedPatterns in NoEmbedPatternslist:
      	for M in Mlist:
	    for T in Tlist:
		directory = "./Episodes/ChainEpisodes/Parser/Debug"
        	#directory = "./Episodes/ChainEpisodes/Parser/%.3f-%.3f-%.3f-%d-%de3-%d-%d-node" % (rho,pee,eta,M,T,NoEmbedPatterns,SizeInputPatterns)
		ipfile = "./datasets/ChainEpisodes/InputSequence-Debug"
        	#ipfile = "./datasets/ChainEpisodes/stream-%.3f-%.3f-%.3f-%d-%de3-%d-%d-node" % (rho,pee,eta,M,T,NoEmbedPatterns,SizeInputPatterns)
		print ipfile
      		try:
          		os.mkdir(directory)
      		except OSError:
      	  		print directory, 'already exists.'
      		for freq in freqlist:
        		for ent in entlist:
          			for exp in explist:
            				opfile = "%s/freq-temp"
            				command = "%s %s %d %.3f %.3f %s | tee %s/freq-exp%.3f" % (miner,ipfile,freq,ent,exp,opfile,directory,exp)
            				os.system(command)
