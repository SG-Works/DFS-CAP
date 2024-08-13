
#Experiment 1 : varying rho
python3 Discovery_synthetic.py ./../../DataSynthetic2/Dataspecs1_Dataset_M=60_N=2_rho=0.01_p=0.03_eta=0.5_T=10000_evnt_strm 20 185
python3 Discovery_synthetic.py ./../../DataSynthetic2/Dataspecs1_Dataset_M=60_N=2_rho=0.02_p=0.03_eta=0.5_T=10000_evnt_strm 20 185
python3 Discovery_synthetic.py ./../../DataSynthetic2/Dataspecs1_Dataset_M=60_N=2_rho=0.03_p=0.03_eta=0.5_T=10000_evnt_strm 20 185
python3 Discovery_synthetic.py ./../../DataSynthetic2/Dataspecs1_Dataset_M=60_N=2_rho=0.035_p=0.03_eta=0.5_T=10000_evnt_strm 20 185
python3 Discovery_synthetic.py ./../../DataSynthetic2/Dataspecs1_Dataset_M=60_N=2_rho=0.04_p=0.03_eta=0.5_T=10000_evnt_strm 20 185
python3 Discovery_synthetic_1run.py ./../../DataSynthetic2/Dataspecs1_Dataset_M=60_N=2_rho=0.045_p=0.03_eta=0.5_T=10000_evnt_strm 20 185
python3 Discovery_synthetic_1run.py ./../../DataSynthetic2/Dataspecs1_Dataset_M=60_N=2_rho=0.05_p=0.03_eta=0.5_T=10000_evnt_strm 20 185

#Experiment 2 : Varying eta
python3 Discovery_synthetic.py ./../../DataSynthetic2/Dataspecs1_Dataset_M=60_N=2_rho=0.03_p=0.03_eta=0.6_T=10000_evnt_strm 17 192
python3 Discovery_synthetic.py ./../../DataSynthetic2/Dataspecs1_Dataset_M=60_N=2_rho=0.03_p=0.03_eta=0.55_T=10000_evnt_strm 19 189
python3 Discovery_synthetic.py ./../../DataSynthetic2/Dataspecs1_Dataset_M=60_N=2_rho=0.03_p=0.03_eta=0.5_T=10000_evnt_strm 20 185
python3 Discovery_synthetic.py ./../../DataSynthetic2/Dataspecs1_Dataset_M=60_N=2_rho=0.03_p=0.03_eta=0.45_T=10000_evnt_strm 23 180
python3 Discovery_synthetic.py ./../../DataSynthetic2/Dataspecs1_Dataset_M=60_N=2_rho=0.03_p=0.03_eta=0.4_T=10000_evnt_strm 25 175
python3 Discovery_synthetic.py ./../../DataSynthetic2/Dataspecs1_Dataset_M=60_N=2_rho=0.03_p=0.03_eta=0.37_T=10000_evnt_strm 28 171
python3 Discovery_synthetic.py ./../../DataSynthetic2/Dataspecs1_Dataset_M=60_N=2_rho=0.03_p=0.03_eta=0.35_T=10000_evnt_strm 29 168

#Experiments 3 : Varying N_embd
python3 Discovery_synthetic.py ./../../DataSynthetic2/DataspecsN1_Dataset_M=60_N=1_rho=0.02_p=0.02_eta=0.5_T=10000_evnt_strm 20 134
python3 Discovery_synthetic.py ./../../DataSynthetic2/DataspecsN2_Dataset_M=60_N=2_rho=0.02_p=0.02_eta=0.5_T=10000_evnt_strm 20 134
python3 Discovery_synthetic.py ./../../DataSynthetic2/DataspecsN3_Dataset_M=60_N=3_rho=0.02_p=0.02_eta=0.5_T=10000_evnt_strm 20 134
python3 Discovery_synthetic.py ./../../DataSynthetic2/DataspecsN4_Dataset_M=60_N=4_rho=0.02_p=0.02_eta=0.5_T=10000_evnt_strm 20 134
python3 Discovery_synthetic.py ./../../DataSynthetic2/DataspecsN5_Dataset_M=60_N=5_rho=0.02_p=0.02_eta=0.5_T=10000_evnt_strm 20 134

#Experiment 4 : Varying Length
python3 Discovery_synthetic.py ./../../DataSynthetic2/DataspecsL4_Dataset_M=60_N=2_rho=0.03_p=0.02_eta=0.5_T=10000_evnt_strm 12 143
python3 Discovery_synthetic.py ./../../DataSynthetic2/DataspecsL5_Dataset_M=60_N=2_rho=0.03_p=0.02_eta=0.5_T=10000_evnt_strm 16 138
python3 Discovery_synthetic.py ./../../DataSynthetic2/DataspecsL6_Dataset_M=60_N=2_rho=0.03_p=0.02_eta=0.5_T=10000_evnt_strm 20 134
python3 Discovery_synthetic.py ./../../DataSynthetic2/DataspecsL7_Dataset_M=60_N=2_rho=0.03_p=0.02_eta=0.5_T=10000_evnt_strm 24 130



#Experiment 5 : Real Data : whale.txt varying tx
python3 NOX_DFS_CE_results_real.py ./../../DataReal/whale_data.txt 5 30
python3 NOX_DFS_CE_results_real.py ./../../DataReal/whale_data.txt 6 30
python3 NOX_DFS_CE_results_real.py ./../../DataReal/whale_data.txt 8 30
python3 NOX_DFS_CE_results_real.py ./../../DataReal/whale_data.txt 10 30
python3 NOX_DFS_CE_results_real.py ./../../DataReal/whale_data.txt 12 30
python3 NOX_DFS_CE_results_real.py ./../../DataReal/whale_data.txt 15 30

#Experiment 6 : Real Data : whale.txt varying ft
python3 NOX_DFS_CE_results_real.py ./../../DataReal/whale_data.txt 10 50
python3 NOX_DFS_CE_results_real.py ./../../DataReal/whale_data.txt 10 40
python3 NOX_DFS_CE_results_real.py ./../../DataReal/whale_data.txt 10 30
python3 NOX_DFS_CE_results_real.py ./../../DataReal/whale_data.txt 10 20
python3 NOX_DFS_CE_results_real.py ./../../DataReal/whale_data.txt 10 15
python3 NOX_DFS_CE_results_real.py ./../../DataReal/whale_data.txt 10 10

#Experiment 7 : Real Data : address.txt varying tx
python3 NOX_DFS_CE_results_real.py ./../../DataReal/address_data.txt 5 100
python3 NOX_DFS_CE_results_real.py ./../../DataReal/address_data.txt 6 100
python3 NOX_DFS_CE_results_real.py ./../../DataReal/address_data.txt 8 100
python3 NOX_DFS_CE_results_real.py ./../../DataReal/address_data.txt 10 100
python3 NOX_DFS_CE_results_real.py ./../../DataReal/address_data.txt 12 100
python3 NOX_DFS_CE_results_real.py ./../../DataReal/address_data.txt 15 100

#Experiment 8 : Real Data : address.txt varying ft
python3 NOX_DFS_CE_results_real.py ./../../DataReal/address_data.txt 10 150
python3 NOX_DFS_CE_results_real.py ./../../DataReal/address_data.txt 10 125
python3 NOX_DFS_CE_results_real.py ./../../DataReal/address_data.txt 10 100
python3 NOX_DFS_CE_results_real.py ./../../DataReal/address_data.txt 10 80
python3 NOX_DFS_CE_results_real.py ./../../DataReal/address_data.txt 10 60
python3 NOX_DFS_CE_results_real.py ./../../DataReal/address_data.txt 10 50
