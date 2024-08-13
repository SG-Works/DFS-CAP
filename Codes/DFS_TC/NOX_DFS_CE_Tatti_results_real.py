#!/usr/bin/env python
# coding: utf-8

# In[1]:


import time
import sys

# In[2]:


class def_episode:
    def __init__(self, evnts, edge_set, lastevnt):
        self.freq = 0
        self.evnts = evnts.copy()
        self.edges = edge_set.copy()
        self.lastevnt = lastevnt


# In[3]:


def PCENOX_Span_Recursive(parent_epi,AO_list_parent,AO_windows_parent,Tx,FT):
    global candidates, frequents
    
    lastevntid = alph.index(parent_epi.lastevnt)
    for new_evnt in alph[lastevntid:]:
        candidates += 1
        if new_evnt == parent_epi.lastevnt:
            repeat = True
            newedges = set()
            for k in range(len(parent_epi.evnts)):
                if parent_epi.evnts[k] == new_evnt:
                    newedges.add((k,len(parent_epi.evnts)))
            edgeset = parent_epi.edges|newedges
        else:
            repeat = False
            edgeset = parent_epi.edges.copy()
        
        AO_list_child, AO_windows_child = Compute_AO_Span_list_PCE(AO_list_parent, AO_windows_parent, new_evnt, repeat, Tx)
        NO_freq_child = Find_NO_Count(AO_windows_child)
        
        if NO_freq_child >= FT:
            frequents += 1
            child_evnts = parent_epi.evnts+[new_evnt]
            child_epi = def_episode(child_evnts,edgeset,new_evnt)
            child_epi.freq = NO_freq_child
            Freq_ep.append(child_epi)
            CENOX_Span_Recursive(child_epi,AO_list_child,AO_windows_child,set(),Tx,FT)
            PCENOX_Span_Recursive(child_epi,AO_list_child,AO_windows_child,Tx,FT)

#         child_evnts = parent_epi.evnts+[new_evnt]
#         child_epi = def_episode(child_evnts,set(),new_evnt)
#         Checkclosure(child_epi)
#         IGENOX_Span_Recursive(child_epi,list(),list(),set(),Tx,FT)
#         IPENOX_Span_Recursive(child_epi,list(),list(),Tx,FT)


# In[4]:


def CENOX_Span_Recursive(parent_epi,AO_list_parent,AO_windows_parent,forbidden_edges,Tx,FT):
    global candidates, frequents
    
    for evnt1_id in range(len(parent_epi.evnts)):
        for evnt2_id in range(len(parent_epi.evnts)):
            new_edge = (evnt1_id,evnt2_id)
            if evnt1_id == evnt2_id:
                continue
            if new_edge in parent_epi.edges:
                continue
            if new_edge in forbidden_edges:
                continue
            if ValidateEdges(parent_epi.edges,new_edge) == True:
                candidates += 1
                AO_list_child, AO_windows_child = Compute_AO_Span_list_CE(AO_list_parent, AO_windows_parent, evnt1_id, evnt2_id)
                NO_freq_child = Find_NO_Count(AO_windows_child)
                
                if NO_freq_child >= FT:
                    frequents += 1
                    child_epi = def_episode(parent_epi.evnts,parent_epi.edges|{new_edge},parent_epi.lastevnt)
                    child_epi.freq = NO_freq_child
                    Checkclosure(child_epi)
                    CENOX_Span_Recursive(child_epi,AO_list_child,AO_windows_child,forbidden_edges,Tx,FT)
                
#                 child_epi = def_episode(parent_epi.evnts,parent_epi.edges|{new_edge},parent_epi.lastevnt)
#                 Checkclosure(child_epi)
#                 IGENOX_Span_Recursive(child_epi,list(),list(),forbidden_edges,Tx,FT)

                forbidden_edges = forbidden_edges|{new_edge}


# In[5]:


def ValidateEdges(edges, new_edge):
    e1 = new_edge[0]
    e2 = new_edge[1]
    if (e2,e1) in edges:
        return False
    for edge in edges:
        if edge[1] == e1 and (edge[0], e2) not in edges:
            return False
        if edge[0] == e2 and (e1, edge[1]) not in edges:
            return False
        
    return True


# In[6]:


def Compute_AO_Span_list_PCE(AO_list_parent, AO_windows_parent, new_event, repeat, Tx):
    global AO_list
    AO_list_child = list()
    AO_windows_child = list()
    for k in range(len(AO_list_parent)):
        occ_tuple = AO_list_parent[k]
        occ_win = AO_windows_parent[k]
        if repeat:
            st = occ_tuple[-1]+1
        else:
            st = occ_win[1]-Tx
        end = occ_win[0]+Tx+1
        for tm in range(st,end):
            if tm in AO_list[new_event]:
                AO_list_child.append(occ_tuple+[tm])
                AO_windows_child.append((min(occ_win[0],tm),max(occ_win[1],tm)))
    sorted_id = sorted(range(len(AO_windows_child)),key=AO_windows_child.__getitem__)
    AO_list_child_sorted = list([])
    AO_windows_child_sorted = list([])
    for sid in sorted_id:
        AO_list_child_sorted.append(AO_list_child[sid])
        AO_windows_child_sorted.append(AO_windows_child[sid])
    return AO_list_child_sorted, AO_windows_child_sorted


# In[7]:


def Compute_AO_Span_list_CE(AO_list_parent, AO_windows_parent, eid1, eid2):
    AO_list_child_sorted = list([])
    AO_windows_child_sorted = list([])
    for sid in range(len(AO_list_parent)):
        if AO_list_parent[sid][eid1] < AO_list_parent[sid][eid2]:
            AO_list_child_sorted.append(AO_list_parent[sid])
            AO_windows_child_sorted.append(AO_windows_parent[sid])
    return AO_list_child_sorted, AO_windows_child_sorted


# In[8]:


def Find_NO_Count(ep_AO_win):
    l = len(ep_AO_win)
    if l == 0:
        return l
#     ts = -1
    te = -1
    fno = 0
    for win in ep_AO_win:
        if win[0] > te:
            fno += 1
            te = win[1]
        elif win[1] < te:
            te = win[1]
    return fno


# In[9]:


def Checkclosure(new_epi):
    global Freq_ep
    p = 0
    len_Freq_ep = len(Freq_ep)
    while p < len_Freq_ep:
        if Freq_ep[p].freq == new_epi.freq:
            if new_epi.evnts == Freq_ep[p].evnts:
                if new_epi.edges.issubset(Freq_ep[p].edges):
                    return
                if Freq_ep[p].edges.issubset(new_epi.edges):
                    Freq_ep.remove(Freq_ep[p])
                    len_Freq_ep -= 1 #len(Freq_ep)
                    p = -1
        p += 1
    Freq_ep.append(new_epi)


# In[10]:


global alph, evnt_id, AO_list

evnt_strm = list()

# print('Enter the link of the txt file containing the event stream (for example: ./../../DataSynthetic/Dataspecs1_Dataset_M=60_N=2_rho=0.055_p=0.07_eta=0.7_T=10000_evnt_strm_set=0.txt)')
# name = input()
# print('Enter the Expiry time "Tx": (Better option would be (lenght of the longest pattern)/eta)')
# Tx = int(input())
# print('Enter the frequency threshold "FT": (Better option would be T/(1/p+Tx) -1)')
# FT = int(input()) 


name = sys.argv[1]
Tx = int(sys.argv[2])
FT = int(sys.argv[3])

#name = './../../DataReal/address_data.txt'
# name = './../../DataReal/whale_data.txt'
#Tx = 10
#FT = 100

print("Dataset: "+ name)
print("Expiry time: "+ str(Tx))
print("Frequency Threshold: "+ str(FT))


# In[11]:


global candidates, frequents, Freq_ep

N_set = 10
avg_time = 0
avg_mem = 0
for n_set in range(N_set):
    
    time_start = time.time()
    
    print("Dataset: "+ name)
    print("Expiry time: "+ str(Tx))
    print("Frequency Threshold: "+ str(FT))
    
    
    alph_org = []
    evnt_id = {}
    AO_list = {}
    count = 0
    with open(name,'r') as f:
        for line in f:
            entry = line.split(',')
            event = entry[0]
            tm = int(entry[1][:-1])
            if event not in alph_org:
                alph_org.append(event)
                evnt_id[event] = count

                AO_list[event] = set()
                count += 1
            AO_list[event].add(tm)

    alph = list()
    F1_ep = list()
    freq_alph = list()
    for ev in alph_org:
        fr = len(AO_list[ev])
        if fr >= FT:
            freq_alph.append([fr,ev])


    Freq_ep = list()
    for (fr,ev) in freq_alph:
        alph.append(ev)
        F1_ep.append(def_episode([ev],set(),ev))
        F1_ep[-1].freq = fr
        Freq_ep.append(def_episode([ev],set(),ev))
        Freq_ep[-1].freq = fr


    candidates = len(alph_org)
    frequents = len(F1_ep)
    for parent_epi in F1_ep:
        Freq_ep.append(parent_epi)
        E = parent_epi.lastevnt
        AO_list_E = list(AO_list[E])
        AO_list_E.sort()
        AO_windows_E = list(map(lambda el:(el,el), AO_list_E))
        AO_list_E = list(map(lambda el:[el], AO_list_E))
        PCENOX_Span_Recursive(parent_epi,AO_list_E,AO_windows_E,Tx,FT)


    print("Number of frequent episodes is ", len(Freq_ep))
    # for ep in Freq_ep:
    #     print(ep.freq ,':', ep.evnts, ep.edges)

    # Freq_closed_ep = Find_Closed(Freq_ep)

    time_end = time.time()
    time_run = time_end-time_start
    print("time: ", time_run)
    avg_time += time_run
    # print("Number of closed frequent episodes is ", len(Freq_closed_ep))

    # print('The following are the closed frequent episodes discovered')
    # for ep in Freq_closed_ep:
    #     print(ep.freq ,':', ep.evnts, ep.edges) 

avg_time = avg_time/N_set
print("Average Run time: ", avg_time)


# In[ ]:


# print(candidates, frequents, len(Freq_ep))


# In[ ]:


# for ep in Freq_ep:
#     if len(ep.evnts) == 6:
#         print(ep.freq ,':', ep.evnts, '\n', ep.edges, '\n')

