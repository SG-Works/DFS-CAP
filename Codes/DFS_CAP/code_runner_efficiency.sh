#!/bin/bash

#Experiment 1 : varying eta
python3 NOX_DFS_CE_results_efficiency.py ./../../DataEfficiency/Dataspecs1_Dataset_M=60_N=2_rho=0.01_p=0.02_eta=0.4_T=10000_evnt_strm 15 128
python3 NOX_DFS_CE_results_efficiency.py ./../../DataEfficiency/Dataspecs1_Dataset_M=60_N=2_rho=0.01_p=0.02_eta=0.4_T=10000_evnt_strm 20 128
python3 NOX_DFS_CE_results_efficiency.py ./../../DataEfficiency/Dataspecs1_Dataset_M=60_N=2_rho=0.01_p=0.02_eta=0.4_T=10000_evnt_strm 25 128

python3 NOX_DFS_CE_results_efficiency.py ./../../DataEfficiency/Dataspecs1_Dataset_M=60_N=2_rho=0.01_p=0.02_eta=0.5_T=10000_evnt_strm 15 134
python3 NOX_DFS_CE_results_efficiency.py ./../../DataEfficiency/Dataspecs1_Dataset_M=60_N=2_rho=0.01_p=0.02_eta=0.5_T=10000_evnt_strm 20 134
python3 NOX_DFS_CE_results_efficiency.py ./../../DataEfficiency/Dataspecs1_Dataset_M=60_N=2_rho=0.01_p=0.02_eta=0.5_T=10000_evnt_strm 25 134

python3 NOX_DFS_CE_results_efficiency.py ./../../DataEfficiency/Dataspecs1_Dataset_M=60_N=2_rho=0.01_p=0.02_eta=0.6_T=10000_evnt_strm 15 138
python3 NOX_DFS_CE_results_efficiency.py ./../../DataEfficiency/Dataspecs1_Dataset_M=60_N=2_rho=0.01_p=0.02_eta=0.6_T=10000_evnt_strm 20 138
python3 NOX_DFS_CE_results_efficiency.py ./../../DataEfficiency/Dataspecs1_Dataset_M=60_N=2_rho=0.01_p=0.02_eta=0.6_T=10000_evnt_strm 25 138

#Experiment 2 : Varying p
python3 NOX_DFS_CE_results_efficiency.py ./../../DataEfficiency/Dataspecs1_Dataset_M=60_N=2_rho=0.02_p=0.02_eta=0.5_T=10000_evnt_strm 15 134
python3 NOX_DFS_CE_results_efficiency.py ./../../DataEfficiency/Dataspecs1_Dataset_M=60_N=2_rho=0.02_p=0.02_eta=0.5_T=10000_evnt_strm 20 134
python3 NOX_DFS_CE_results_efficiency.py ./../../DataEfficiency/Dataspecs1_Dataset_M=60_N=2_rho=0.02_p=0.02_eta=0.5_T=10000_evnt_strm 25 134

python3 NOX_DFS_CE_results_efficiency.py ./../../DataEfficiency/Dataspecs1_Dataset_M=60_N=2_rho=0.02_p=0.03_eta=0.5_T=10000_evnt_strm 15 185
python3 NOX_DFS_CE_results_efficiency.py ./../../DataEfficiency/Dataspecs1_Dataset_M=60_N=2_rho=0.02_p=0.03_eta=0.5_T=10000_evnt_strm 20 185
python3 NOX_DFS_CE_results_efficiency.py ./../../DataEfficiency/Dataspecs1_Dataset_M=60_N=2_rho=0.02_p=0.03_eta=0.5_T=10000_evnt_strm 25 185

python3 NOX_DFS_CE_results_efficiency.py ./../../DataEfficiency/Dataspecs1_Dataset_M=60_N=2_rho=0.02_p=0.04_eta=0.5_T=10000_evnt_strm 15 229
python3 NOX_DFS_CE_results_efficiency.py ./../../DataEfficiency/Dataspecs1_Dataset_M=60_N=2_rho=0.02_p=0.04_eta=0.5_T=10000_evnt_strm 20 229
python3 NOX_DFS_CE_results_efficiency.py ./../../DataEfficiency/Dataspecs1_Dataset_M=60_N=2_rho=0.02_p=0.04_eta=0.5_T=10000_evnt_strm 25 229

#Experiments 3 : Varying rho
python3 NOX_DFS_CE_results_efficiency.py ./../../DataEfficiency/Dataspecs1_Dataset_M=60_N=2_rho=0.01_p=0.04_eta=0.6_T=10000_evnt_strm 15 240
python3 NOX_DFS_CE_results_efficiency.py ./../../DataEfficiency/Dataspecs1_Dataset_M=60_N=2_rho=0.01_p=0.04_eta=0.6_T=10000_evnt_strm 20 240
python3 NOX_DFS_CE_results_efficiency.py ./../../DataEfficiency/Dataspecs1_Dataset_M=60_N=2_rho=0.01_p=0.04_eta=0.6_T=10000_evnt_strm 25 240

python3 NOX_DFS_CE_results_efficiency.py ./../../DataEfficiency/Dataspecs1_Dataset_M=60_N=2_rho=0.02_p=0.04_eta=0.6_T=10000_evnt_strm 15 240
python3 NOX_DFS_CE_results_efficiency.py ./../../DataEfficiency/Dataspecs1_Dataset_M=60_N=2_rho=0.02_p=0.04_eta=0.6_T=10000_evnt_strm 20 240
python3 NOX_DFS_CE_results_efficiency.py ./../../DataEfficiency/Dataspecs1_Dataset_M=60_N=2_rho=0.02_p=0.04_eta=0.6_T=10000_evnt_strm 25 240

python3 NOX_DFS_CE_results_efficiency.py ./../../DataEfficiency/Dataspecs1_Dataset_M=60_N=2_rho=0.03_p=0.04_eta=0.6_T=10000_evnt_strm 15 240
python3 NOX_DFS_CE_results_efficiency.py ./../../DataEfficiency/Dataspecs1_Dataset_M=60_N=2_rho=0.03_p=0.04_eta=0.6_T=10000_evnt_strm 20 240
python3 NOX_DFS_CE_results_efficiency.py ./../../DataEfficiency/Dataspecs1_Dataset_M=60_N=2_rho=0.03_p=0.04_eta=0.6_T=10000_evnt_strm 25 240


