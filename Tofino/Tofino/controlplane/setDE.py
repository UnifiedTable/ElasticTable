'''
    UnifedTable Tofino demo with SDE 9.7
    This program is used to generate the Tofino demo figure in our paper.

    From the author: 
    The code doesn't follow a well-written Python style. 
    That's because the run_bfshell.sh can only execute one python file(import is not useful)
    and cannot execute any code in 'def' fucntions, so I write this program without any self-defined
    functions and objects. I'm not sure if it is because of our switch environment or SDE's own bugs.
    I'm sorry for this poorly-written style.
'''
import json
import time

#config
DESize = 1<<10
macFilePath = ""
arpFilePath = ""
fibFilePath = ""
aclFilePath = ""
macDSLength = 5
fibDSLength = 20
aclDSLength = 10

# data structure
IngressOb = bfrt.onChip.pipe.Ingress

flagReg = IngressOb.flag_reg
macArray = []
fibArray = []
aclSrcArray = []
aclDstArray = []

# controlplane init
with open(macFilePath, "r") as macFile:
    for i in range(macDSLength):
        macArray.append([])
        for j in range(2):
            macArray[i].append([])
            for k in range(DESize):
                data = int(macFile.readline().strip())
                macArray[i][j].append(data)

with open(fibFilePath, "r") as fibFile:
    for i in range(fibDSLength):
        fibArray.append([])
        for j in range(2):
            fibArray[i].append([])
            for k in range(DESize):
                data = int(fibFile.readline().strip())
                fibArray[i][j].append(data)

with open(aclFilePath, "r") as aclFilePath:
    for i in range(aclDSLength):
        aclSrcArray.append([])
        for j in range(2):
            aclSrcArray[i].append([])
            for k in range(DESize):
                data = int(aclFilePath.readline().strip())
                aclSrcArray[i][j].append(data)
    for i in range(aclDSLength):
        aclDstArray.append([])
        for j in range(2):
            aclDstArray[i].append([])
            for k in range(DESize):
                data = int(aclFilePath.readline().strip())
                aclDstArray[i][j].append(data)

# begin test 

# user interaction info
print("Please run the dpdk controller, then press Entry to continue demo")
input()
print("Please run the dpdk sender, then press Enter to continue demo")
print("Sender will send packets with a specific speed")
input()
print("Please run the dpdk recevier, then press Enter to continue demo")
print("Recevier will show the number of packets per second(throughput)")
input()
print("Begin demo, next steps are automatic")

# step 1: mac test
# init mac table
print("begin init dataplane MAC address table")
print("*****Step 1: MAC forward through controlplane******")
start_time = time.time()
flagReg.mod(0, 0b1000)
# init diamond embedder module(default: 5 parallel diamond embedders)
for idx in range(20):
    table_name = f"calculate_key_table_{idx+1}"
    func_name = f"set_default_with_NoAction"
    tbl = getattr(IngressOb, table_name)
    func = getattr(tbl, func_name)
    func()
    for p in range(2):
        de_name = f"diamond_embedder_{idx+1}_{p+1}"
        de = getattr(IngressOb, de_name)
        de.clear()

for idx in range(macDSLength):
    table_name = f"calculate_key_table_{idx+1}"
    func_name = f"set_default_with_calculate_key_{idx+1}_mac"
    tbl = getattr(IngressOb, table_name)
    func = getattr(tbl, func_name)
    func((1<<(idx+1))-1)
    for p in range(2):
        for i in range(DESize):
            de_name = f"diamond_embedder_{idx+1}_{p+1}"
            de = getattr(IngressOb, de_name)
            de.mod(i, macArray[idx][p][i])
end_time = time.time()
print("Init dataplane MAC forward module time: %f s" %(end_time-start_time))
time.sleep(1)



print("******Step 2: MAC completely dataplane forward******")
# init flag reg, open MAC forward module
flagReg.mod(0, 0b1)
time.sleep(10)
print("finish mac forward test")


#step 3: LPM forward test
print("******Step 3: FIB forward without dataplane accelerate******")
start_time = time.time()
flagReg.mod(0, 0b10000)
for idx in range(20):
    table_name = f"calculate_key_table_{idx+1}"
    func_name = f"set_default_with_NoAction"
    tbl = getattr(IngressOb, table_name)
    func = getattr(tbl, func_name)
    func()
    for p in range(2):
        de_name = f"diamond_embedder_{idx+1}_{p+1}"
        de = getattr(IngressOb, de_name)
        de.clear()

for idx in range(fibDSLength):
    table_name = f"calculate_key_table_{idx+1}"
    func_name = f"set_default_with_calculate_key_{idx+1}_dstip"
    tbl = getattr(IngressOb, table_name)
    func = getattr(tbl, func_name)
    func((1<<(idx+1))-1)
    for p in range(2):
        for i in range(DESize):
            de_name = f"diamond_embedder_{idx+1}_{p+1}"
            de = getattr(IngressOb, de_name)
            de.mod(i, fibArray[idx][p][i])
end_time = time.time()
print("Init dataplane FIB module time: %f s" %(end_time-start_time))

# begin FIB dataplane accelerate
print("******Step 4: FIB forward with dataplane accelerate******")
flagReg.mod(0, 0b10)
time.sleep(10)
print("finish FIB forward test")

#step 4: ACL forward test
print("******Step 5: ACL forward without dataplane accelerate******")
start_time = time.time()
flagReg.mod(0, 0b100000)
for idx in range(20):
    table_name = f"calculate_key_table_{idx+1}"
    func_name = f"set_default_with_NoAction"
    tbl = getattr(IngressOb, table_name)
    func = getattr(tbl, func_name)
    func()
    for p in range(2):
        de_name = f"diamond_embedder_{idx+1}_{p+1}"
        de = getattr(IngressOb, de_name)
        de.clear()

for idx in range(aclDSLength):
    table_name = f"calculate_key_table_{idx+1}"
    func_name = f"set_default_with_calculate_key_{idx+1}_srcip"
    tbl = getattr(IngressOb, table_name)
    func = getattr(tbl, func_name)
    func((1<<(idx+1))-1)
    for p in range(2):
        for i in range(DESize):
            de_name = f"diamond_embedder_{idx+1}_{p+1}"
            de = getattr(IngressOb, de_name)
            de.mod(i, aclSrcArray[idx][p][i])

for idx in range(aclDSLength):
    table_name = f"calculate_key_table_{idx+11}"
    func_name = f"set_default_with_calculate_key_{idx+11}_srcip"
    tbl = getattr(IngressOb, table_name)
    func = getattr(tbl, func_name)
    func((1<<(idx+1))-1)
    for p in range(2):
        for i in range(DESize):
            de_name = f"diamond_embedder_{idx+11}_{p+1}"
            de = getattr(IngressOb, de_name)
            de.mod(i, aclDstArray[idx][p][i])
end_time = time.time()
print("Init dataplane ACL module time: %f s" %(end_time-start_time))

# begin ACL dataplane accelerate
print("Step 5: ACL forward with dataplane accelerate")
flagReg.mod(0, 0b100)
time.sleep(10)
print("finish forward test")
print("Please kill the receiver and the sender to get the result")
