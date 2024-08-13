#!/usr/bin/env python
# coding: utf-8

# In[1]:


import time
import sys
# from math import log2


# In[2]:


class def_episode:
    def __init__(self, evnts, edge_set, lastevnt, pred, succ):
        self.freq = 0
        self.evnts = evnts.copy()
        self.edges = edge_set.copy()
        self.lastevnt = lastevnt
        self.pred = pred.copy()
        self.succ = succ.copy()
        self.fclosed = True
        # self.appclosed = True 


# In[3]:


def CENOX_Span_Recursive(ParentBlock,AO_list_parent,AO_windows_parent,Tx,FT):
    global candidates, frequents, Freq_ep
    
    closed = True
    ChildBlock = list()
    parent_epi = ParentBlock[-1]
    last_event = parent_epi.lastevnt
    parent_len = len(parent_epi.evnts)
    
    for helper_epi in ParentBlock:
        
        new_event = helper_epi.lastevnt
        Y0_events = parent_epi.evnts + [new_event]
        # Y0_edges = parent_epi.edges + helper_epi.edges
        Y0_edges = parent_epi.edges.copy()
        pred = helper_epi.pred
        succ = helper_epi.succ
        for ev_id in pred:
            Y0_edges.add((ev_id,parent_len))
        for ev_id in succ:
            Y0_edges.add((parent_len,ev_id))
        
        
        ValidEdges = ValidateEdges(parent_epi.pred,parent_epi.succ,helper_epi.pred,helper_epi.succ)

        if parent_epi.lastevnt != helper_epi.lastevnt:
            
            candidates += sum(ValidEdges)

            if ValidEdges[0] or (ValidEdges[1] and ValidEdges[2]):
                AO_list_Y0, AO_windows_Y0 = Compute_AO_Span_list(AO_list_parent, AO_windows_parent, new_event, pred, succ, Tx)
                NO_freq_Y0 = Find_NO_Count(AO_windows_Y0)
                # NO_freq_Y0, BE_Y0 = Find_NOBE_Count(AO_list_Y0, AO_windows_Y0)
                # if Y0_events == ['5','5','3'] and Y0_edges == {(0,1)}:
                #     NO_freq_Y0, BE_Y0 = Find_NOBE_Count2(AO_list_Y0, AO_windows_Y0)
                #     # NO_freq_Y0, BE_Y0 = Find_NOBE_Count(AO_list_Y0, AO_windows_Y0)
                # else:
                #     NO_freq_Y0, BE_Y0 = Find_NOBE_Count(AO_list_Y0, AO_windows_Y0)
                if NO_freq_Y0 < FT:
                    continue

            AO_list_Y1 = list()
            AO_windows_Y1 = list()
            AO_list_Y2 = list()
            AO_windows_Y2 = list()

            if ValidEdges[1] and ValidEdges[2]:
                for k in range(len(AO_list_Y0)):
                    if AO_list_Y0[k][-2] < AO_list_Y0[k][-1]:
                        AO_list_Y1.append(AO_list_Y0[k])
                        AO_windows_Y1.append(AO_windows_Y0[k])
                    elif AO_list_Y0[k][-2] > AO_list_Y0[k][-1]:
                        AO_list_Y2.append(AO_list_Y0[k])
                        AO_windows_Y2.append(AO_windows_Y0[k])
            elif ValidEdges[0]:
                if ValidEdges[1]:
                    for k in range(len(AO_list_Y0)):
                        if AO_list_Y0[k][-2] < AO_list_Y0[k][-1]:
                            AO_list_Y1.append(AO_list_Y0[k])
                            AO_windows_Y1.append(AO_windows_Y0[k])
                elif ValidEdges[2]:
                    for k in range(len(AO_list_Y0)):
                        if AO_list_Y0[k][-2] > AO_list_Y0[k][-1]:
                            AO_list_Y2.append(AO_list_Y0[k])
                            AO_windows_Y2.append(AO_windows_Y0[k])
            elif ValidEdges[1]:
                AO_list_Y1, AO_windows_Y1 = Compute_AO_Span_list(AO_list_parent, AO_windows_parent, new_event, pred|{parent_len-1}, succ, Tx)
            elif ValidEdges[2]:
                AO_list_Y2, AO_windows_Y2 = Compute_AO_Span_list(AO_list_parent, AO_windows_parent, new_event, pred, succ|{parent_len-1}, Tx)

            if ValidEdges[1]:
                NO_freq_Y1 = Find_NO_Count(AO_windows_Y1)
                # if Y0_events == ['5','5','3']:
                #     print(NO_freq_Y1,Y0_events, Y0_edges|{(parent_len-1, parent_len)})
                if NO_freq_Y1 >= FT:
                    if NO_freq_Y1 == parent_epi.freq:
                        closed = False
                        parent_epi.fclosed = False
                    if NO_freq_Y1 == helper_epi.freq:
                        helper_epi.fclosed = False
                    # if NO_freq_Y1/parent_epi.freq > 0.5:
                    #     parent_epi.appclosed = False
                    # if NO_freq_Y1/helper_epi.freq > 0.5:
                    #     helper_epi.appclosed = False
                    Y1 = def_episode(Y0_events,Y0_edges|{(parent_len-1, parent_len)}, new_event, pred|{parent_len-1}, succ)
                    Y1.freq = NO_freq_Y1
                    ChildBlock.append(Y1)
                    CENOX_Span_Recursive(ChildBlock,AO_list_Y1,AO_windows_Y1,Tx,FT)
            
            if ValidEdges[0]:# and BE_Y0 > BET:
                # if Y0_events == ['5','5','3'] and Y0_edges == {(0,1)}:
                #     print(NO_freq_Y0, BE_Y0, Y0_events, Y0_edges)
                if NO_freq_Y0 == parent_epi.freq:
                    closed = False
                    parent_epi.fclosed = False
                if NO_freq_Y0 == helper_epi.freq:
                    helper_epi.fclosed = False
                # if NO_freq_Y0/parent_epi.freq > 0.5:
                #     parent_epi.appclosed = False
                # if NO_freq_Y0/helper_epi.freq > 0.5:
                #     helper_epi.appclosed = False
                Y0 = def_episode(Y0_events, Y0_edges, new_event, pred, succ)
                Y0.freq = NO_freq_Y0
                ChildBlock.append(Y0)
                CENOX_Span_Recursive(ChildBlock,AO_list_Y0,AO_windows_Y0,Tx,FT)
            
            if ValidEdges[2]:   
                NO_freq_Y2 = Find_NO_Count(AO_windows_Y2)
                # if Y0_events == ['5','5','3']:
                #     print(NO_freq_Y2,Y0_events, Y0_edges|{(parent_len, parent_len-1)})
                if NO_freq_Y2 >= FT:
                    if NO_freq_Y2 == parent_epi.freq:
                        closed = False
                        parent_epi.fclosed = False
                    if NO_freq_Y2 == helper_epi.freq:
                        helper_epi.fclosed = False
                    # if NO_freq_Y2/parent_epi.freq > 0.5:
                    #     parent_epi.appclosed = False
                    # if NO_freq_Y2/helper_epi.freq > 0.5:
                    #     helper_epi.appclosed = False
                    Y2 = def_episode(Y0_events,Y0_edges|{(parent_len, parent_len-1)}, new_event, pred, succ|{parent_len-1})
                    Y2.freq = NO_freq_Y2
                    ChildBlock.append(Y2)
                    CENOX_Span_Recursive(ChildBlock,AO_list_Y2,AO_windows_Y2,Tx,FT)
        else:
            if ValidEdges[1]:
                candidates += 1
                Y0_edges.add((parent_len-1,parent_len))
                AO_list_Y1, AO_windows_Y1 = Compute_AO_Span_list(AO_list_parent, AO_windows_parent, new_event, pred|{parent_len-1}, succ, Tx)
                NO_freq_Y1 = Find_NO_Count(AO_windows_Y1)
                # if Y0_events == ['5','5','3','3']:
                #     print(parent_epi.evnts, parent_epi.edges)
                #     print(helper_epi.evnts, helper_epi.edges,'\n')
                #     print(NO_freq_Y1,Y0_events, Y0_edges|{(parent_len-1, parent_len)})
                if NO_freq_Y1 >= FT:
                    if NO_freq_Y1 == parent_epi.freq:
                        closed = False
                        parent_epi.fclosed = False
                    if NO_freq_Y1 == helper_epi.freq:
                        helper_epi.fclosed = False
                    # if NO_freq_Y1/parent_epi.freq > 0.5:
                    #     parent_epi.appclosed = False
                    # if NO_freq_Y1/helper_epi.freq > 0.5:
                    #     helper_epi.appclosed = False
                    Y1 = def_episode(Y0_events,Y0_edges|{(parent_len-1, parent_len)}, new_event, pred|{parent_len-1}, succ)
                    Y1.freq = NO_freq_Y1
                    ChildBlock.append(Y1)
                    CENOX_Span_Recursive(ChildBlock,AO_list_Y1,AO_windows_Y1,Tx,FT)

    if closed:
        Freq_ep.append(parent_epi)
    frequents += 1
        
    return Freq_ep


# In[4]:


def ValidateEdges(predp,succp,predh,succh):
    Validity = list([False,False,False])
    if (succp & predh):
        Validity[1] = True
    elif (succh & predp):
        Validity[2] = True
    else:
        Validity[0] = True
        if not (predp-predh | succh-succp):
            Validity[1] = True
        if not (predh-predp | succp-succh):
            Validity[2] = True
    return Validity


# In[5]:


def Compute_AO_Span_list(AO_list_parent, AO_windows_parent, new_event, pred, succ, Tx):
    global AO_list
    AO_list_child = list()
    AO_windows_child = list()
    for k in range(len(AO_list_parent)):
        occ_tuple = AO_list_parent[k]
        occ_win = AO_windows_parent[k]
        if not pred:
            st = occ_win[1]-Tx
        else:
            st = max([occ_tuple[id] for id in pred])+1
        if not succ:
            end = occ_win[0]+Tx+1
        else:
            end = min([occ_tuple[id] for id in succ])
        for tm in range(st,end):
            if tm in AO_list[new_event]:
                AO_list_child.append(occ_tuple+[tm])
                AO_windows_child.append((min(occ_win[0],tm),max(occ_win[1],tm)))
    return AO_list_child, AO_windows_child
                


# In[6]:


def Find_NO_Count(ep_AO_win):
    l = len(ep_AO_win)
    if l == 0:
        return l
    ts = -1
    te = -1
    fno = 0
    for win in ep_AO_win:
        if win[0] > te:
            fno += 1
            ts = te
            te = win[1]
        elif win[0] > ts and win[1] < te:
            te = win[1]
    return fno


# In[7]:


def Find_NOBE_Count(ep_AO_list, ep_AO_win):
    global occs, wins
    occs = ep_AO_list.copy()
    wins = ep_AO_win.copy()
    # for occ in ep_AO_list:
    #     print(occ)
    l = len(ep_AO_win)
    if l == 0:
        return l,l
#     ts,te = ep_AO_win[0]
    ts = -1
    te = ep_AO_win[0][1]
    # forwardflag = ep_AO_list[0][-2] < ep_AO_list[0][-1]
    fno = 1
    forwardcount = 0
    if ep_AO_list[0][-2] < ep_AO_list[0][-1]:
        forwardcount += 1
    for k in range(1,l):
        win = ep_AO_win[k]
        # print(win)
        if ep_AO_list[k][-2] < ep_AO_list[k][-1]:
            forwardcount += 1
        if win[0] > te:
            # if forwardflag:
            #     forwardcount += 1
#             if reverseflag:
#                 reverseflag += 1
            fno += 1
            ts = te
            te = win[1]
            # forwardflag = ep_AO_list[k][-2] < ep_AO_list[k][-1]
        elif win[0] > ts and win[1] < te:
            # forwardflag = ep_AO_list[k][-2] < ep_AO_list[k][-1]
#             reverseflag = ep_AO_list[k][-1] < ep_AO_list[k][-2]
            te = win[1]
    # forwardprob = forwardcount/fno
    
    forwardprob = forwardcount/l
    # print(forwardprob, fno, l)
    if forwardprob == 0 or forwardprob == 1:
        BE = 0
    else:
        BE = -forwardprob*log2(forwardprob)-(1-forwardprob)*log2(1-forwardprob)
    return fno, BE


# In[8]:


def Find_Closed_levelwise(Freq_ep):
    hasher = {}
    hashtable = list()
    count = 0
    for ep in Freq_ep:
        fn = ep.freq
        if fn not in hasher:
            hasher[fn] = count
            hashtable.append(list([ep]))
            count += 1
        else:
            hashtable[hasher[fn]].append(ep)

    Freq_closed_ep = list()
    for k in range(len(hashtable)):
        len_hash_k = len(hashtable[k])
        p = 0
        while p < len_hash_k:
            q = 0
            
            
            while q < p:# and p < len_hash_k:
                # print(p,q,len_hash_k)
                if hashtable[k][p].evnts == hashtable[k][q].evnts:
                    if hashtable[k][p].edges.issubset(hashtable[k][q].edges):
                        hashtable[k].remove(hashtable[k][p])
                        len_hash_k -= 1
                        p -= 1
                        break
                    if hashtable[k][q].edges.issubset(hashtable[k][p].edges):
                        hashtable[k].remove(hashtable[k][q])
                        len_hash_k -= 1 #len(hashtable[k])
                        q -= 1
                        p -= 1
                q += 1
            p += 1
            
            
            
#             if p == q:
#                 q += 1
#             while q < len_hash_k:
#                 if hashtable[k][p].evnts == hashtable[k][q].evnts:
#                     if hashtable[k][p].edges.issubset(hashtable[k][q].edges):
#                         hashtable[k].remove(hashtable[k][p])
#                         len_hash_k -= 1 #len(hashtable[k])
#                         q = -1
#                         break
#                 q += 1
#                 if p == q:
#                     q += 1
                    
#             if q != -1:
#                 p += 1
    
    
    for fc_eps in hashtable:
        Freq_closed_ep += fc_eps

    return Freq_closed_ep

# Freq_closed_ep = Find_Closed_levelwise(Freq2_ep)


# In[9]:


global alph, evnt_id, AO_list

evnt_strm = list()

# print('Enter the link of the txt file containing the event stream (for example: ./../../DataSynthetic/Dataspecs1_Dataset_M=60_N=2_rho=0.055_p=0.07_eta=0.7_T=10000_evnt_strm_set=0.txt)')
# name = input()
# print('Enter the Expiry time "Tx": (Better option would be (lenght of the longest pattern)/eta)')
# Tx = int(input())
# print('Enter the frequency threshold "FT": (Better option would be T/(1/p+Tx) -1)')
# FT = int(input())   
# # print('Enter the Bidirectional Evidence threshold "BET": (Better option would be 0.4)')
# # BET = float(input())   


name = sys.argv[1]
Tx = int(sys.argv[2])
FT = int(sys.argv[3])
# # BET = float(sys.argv[4]) 

#name = './../../DataSynthetic2/Dataspecs1_Dataset_M=60_N=2_rho=0.02_p=0.03_eta=0.5_T=10000_evnt_strm'
#Tx = 20
#FT = 185
# BET = 0.4

print("Dataset: "+ name)
print("Expiry time: "+ str(Tx))
print("Frequency Threshold: "+ str(FT))
#print("Bidirectional Evidence: "+ str(BET))


# In[10]:


global candidates, frequents, Freq_ep

N_set = 10
avg_time = 0
avg_mem = 0
avg_candidates = 0
avg_frequents = 0
avg_closed = 0
avg_closed_6 = 0
for n_set in range(N_set):
    
    time_start = time.time()
    full_name = name + '_set=' + str(n_set) + '.txt'
    
    print("Dataset: "+ full_name)
    print("Expiry time: "+ str(Tx))
    print("Frequency Threshold: "+ str(FT))
    # print("Bidirectional Evidence: "+ str(BET))
    

    alph_org = set()
    evnt_id = {}
    AO_list = {}
    count = 0
    with open(full_name,'r') as f:
        for line in f:
            entry = line.split(',')
            event = entry[0]
            tm = int(entry[1][:-1])
            if event not in alph_org:
                alph_org.add(event)
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

    freq_alph.sort(reverse = True)
    for (fr,ev) in freq_alph:
        alph.append(ev)
        F1_ep.append(def_episode([ev],set(),ev,set(),set()))
        F1_ep[-1].freq = fr

    candidates = len(alph_org)
    frequents = 0
    Freq_ep = list()
    Block = list()
    for parent_epi in F1_ep:
        Block.append(parent_epi)
        E = parent_epi.lastevnt
        AO_list_E = list(AO_list[E])
        AO_list_E.sort()
        AO_windows_E = list(map(lambda el:(el,el), AO_list_E))
        AO_list_E = list(map(lambda el:[el], AO_list_E))
        CENOX_Span_Recursive(Block,AO_list_E,AO_windows_E,Tx,FT)

    Freq2_ep = []
    for ep in Freq_ep:
        if ep.fclosed == True:
            Freq2_ep.append(ep)

    print("Number of frequent episodes is ", len(Freq2_ep))
    # for ep in Freq_ep:
    #     print(ep.freq ,':', ep.evnts, ep.edges)

    Freq_closed_ep = Find_Closed_levelwise(Freq2_ep)

    time_end = time.time()
    time_run = time_end-time_start
    print("Run time: ", time_run)
    avg_time += time_run
    print("Number of closed frequent episodes is ", len(Freq_closed_ep))

    avg_candidates += candidates
    avg_frequents += frequents
    avg_closed += len(Freq_closed_ep)
    for ep in Freq_closed_ep:
        if len(ep.evnts) == 6:
            avg_closed_6 += 1
    # print('The following are the frequent episodes discovered')
    # for ep in Freq_closed_ep:
    #     print(ep.freq ,':', ep.evnts)#, ep.edges) 
    
avg_time = avg_time/N_set

avg_candidates += round(avg_candidates/N_set)
avg_frequents += round(avg_frequents/N_set)
avg_closed += round(avg_closed/N_set)
avg_closed_6 = round(avg_closed_6/N_set)

print("Average Run time: ", avg_time)
print("Avg_CLosed_at_6-Avg_Closed-Avg_Frequents-Avg_Candidates: "+str(avg_closed_6)+'-'+str(avg_closed)+'-'+str(avg_frequents)+'-'+str(avg_candidates))


# In[11]:


# print(candidates, frequents, len(Freq_ep), len(Freq2_ep), len(Freq_closed_ep))


# In[12]:


# for ep in Freq_closed_ep:
#     if len(ep.evnts) == 6:
#         print(ep.freq ,':', ep.evnts, '\n', ep.edges, '\n')


# In[13]:


# count = 0
# for ep in Freq_ep:
#     if ep.fclosed == True:
#         count += 1
# print(candidates,frequents,len(Freq_ep),count,len(Freq_closed_ep))


# In[14]:


# Freq2_ep = []
# for ep in Freq_ep:
#     if ep.fclosed == True:
#         Freq2_ep.append(ep)
# Freq2_closed_ep = Find_Closed_levelwise(Freq2_ep)
# print(candidates, frequents, len(Freq_ep), len(Freq2_ep), len(Freq_closed_ep), len(Freq2_closed_ep))


# In[15]:


# Freq3_ep = []
# for ep in Freq_ep:
#     if ep.appclosed == True:
#         Freq3_ep.append(ep)
# Freq3_closed_ep = Find_Closed_levelwise(Freq3_ep)
# print(len(Freq3_ep), len(Freq3_closed_ep))


# In[16]:


# for ep in Freq3_closed_ep:
#     if len(ep.evnts) == 6:
#         print(ep.freq ,':', ep.evnts, '\n', ep.edges, '\n')


# In[17]:


# sum([True, False, True])

