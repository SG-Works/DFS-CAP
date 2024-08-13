import sys,os,subprocess,time

# datalength = 50000 #our reference
# #episize = 6  #our reference.
# rholist = [0.00]
# peelist = [0.05]
# etalist = [0.5]
# Tlist = [1.0]
# NoEmbedPatternslist = [1]
# SizeInputPatterns = 3
# Mlist = [25]
# # eta = 0.6 to be done later
# # rho = 0.05 needds higher threshold.
# # eta = 0.2 had been done for rho = 0.02 and 0.03

# runs = len(rholist) * len(peelist) * len(etalist) 

miner = "./n-AutoMineGraphs-feb22/bin/n-graph-miner"
i = 1

directory = './Episodes'


name = sys.argv[1]
explist = [float(sys.argv[2])]
freqlist = [float(sys.argv[3])]
BElist = [-1]

N_set = 1
avg_time = 0

for n_set in range(N_set):

  Dataset = name + "_set=" + str(n_set)
  dd = Dataset.split('/')
  ipfile =  Dataset + ".txt"

  try:
       os.mkdir(directory)
  except OSError:
          print (directory, ' already exists.')

# for pee in peelist:
#   for eta in etalist:
#     for rho in rholist:
#       for NoEmbedPatterns in NoEmbedPatternslist:
#       	for M in Mlist:
# 	  for T in Tlist:
#       		directory = "./Episodes/ChainEpisodes/Discovery/%.3f-%.3f-%.3f-%d-%de3-%d-%d-node" % (rho,pee,eta,M,T,NoEmbedPatterns,SizeInputPatterns)
#       		ipfile = "./datasets/ChainEpisodes/stream-%.3f-%.3f-%.3f-%d-%de3-%d-%d-node" % (rho,pee,eta,M,T,NoEmbedPatterns,SizeInputPatterns)
#       		try:
#         		os.mkdir(directory)
#       		except OSError:
#        			print directory, ' already exists.'
#       		freqlist = [30]
#       		BElist = [0.0]
#       		explist = [0.5]
  for freq in freqlist:
    for BE in BElist:
      for exp in explist:
            		# opfile : contains freqEpisodes
            		# logfile : records the display on the stdout to a file.
        time_start = time.time()
        opfile = "%s/%s-freq-%d-exp-%.3f" % (directory,dd[-1],freq,exp)                                                                     
        logfile = "%s/%s-%d-%.3f.log" % (directory,dd[-1],freq,exp)
        fin = open(logfile,'w')
        command = "%s %s %d %.2f %.3f %s" % (miner,ipfile,freq,BE,exp,opfile)
                        # print 'run ',i,' of ',runs
                        # print 'DataGen Parameters : rho = %.3f, pee = %.3f, eta = %.3f, M=%d, T=%de3, N_emb=%d' % (rho,pee,eta,M,T,NoEmbedPatterns)
        print(ipfile)
        print ('Mining Parameters :  expTime = %.3f, freqThresh = %d' % (exp,freq))
                        # create a new shell with 'command' are the command to run.
        process = subprocess.Popen(command, shell=True, stderr=fin, stdout=fin)
                        # following lines do polling on the new shell 
                        # at intervals of 30 seconds and see whether the program
                        # has terminated.
        flag = 0

        while(1):
          time.sleep(0.1)
          if process.poll() != None:
          # come here when process is complete
            print ('')
            time_end = time.time()
            time_run = time_end-time_start
            print ('Run Completed in ', time_run, ' seconds.')
            avg_time += time_run
            #fin.write("Data Gen Parametes : rho = %.3f, pee = %.3f, eta = %.3f, M=%d, T=%de3, N_emb=%d\n"%(rho,pee,eta,M,T,NoEmbedPatterns))
                #fin.write("Program Completed. Running time = %d seconds \n" % (waits*5))
            flag = 1
            break


        # # 61 in the next line is the maximum time i look at the process.
        # for waits in range(1,551):
        #   # 30 here means 30 seconds time interval between peeks.
        #   time.sleep(5)
        #   if process.poll() != None:
        #   # come here when process is complete
        #     print ''
        #     print 'Run Completed in ', waits*5, ' seconds.'
        #     fin.write("Data Gen Parametes : rho = %.3f, pee = %.3f, eta = %.3f, M=%d, T=%de3, N_emb=%d\n"%(rho,pee,eta,M,T,NoEmbedPatterns))
        #     fin.write("Program Completed. Running time = %d minutes\n" % (waits*5))
        #     flag = 1
        #     break
        #   else:
        #     print waits,'... ',
        #     sys.stdout.flush()
        # print ''
        # if flag == 0:
        #   process.kill()
        #   print "Process killed intentionally due to over time."
        #   fin.write("Data Gen Parametes : rho = %.3f, pee = %.3f, eta = %.3f, M=%d, T=%de3, N_emb=%d\n"%(rho,pee,eta,M,T,NoEmbedPatterns))
        #   fin.write("Program Terminated. Time allowed to run : 30 mins.\n")
        # i = i + 1
        fin.close()

avg_time = avg_time/N_set
print("Average Run time: ", avg_time)
