import re

output = open ('slowstartGraph.cwnd','w')
times = []
i = 0
with open('project2SST.cwnd', 'r') as sst:
	for stuff in sst:
		times.append(stuff)

time0 = times[0]
time0 = re.split(r'\t+', time0.rstrip('\t'))
time1 = times[1]
time1 = re.split(r'\t+', time1.rstrip('\t'))
time2 = times[2]
time2 = re.split(r'\t+', time2.rstrip('\t'))
time3 = times[3]
time3 = re.split(r'\t+', time3.rstrip('\t'))
time4 = times[4]
time4 = re.split(r'\t+', time4.rstrip('\t'))
time5 = times[5]
time5 = re.split(r'\t+', time5.rstrip('\t'))
time6 = times[6]
time6 = re.split(r'\t+', time6.rstrip('\t'))
time7 = times[7]
time7 = re.split(r'\t+', time7.rstrip('\t'))

with open('project2.cwnd','r') as cwnd:
	for line in cwnd:
		strs = re.split(r'\t+', line.rstrip('\t'))
		
		if (float(strs[0]) >= float(time0[0])) and (float(strs[0])<float(time1[0])):
		
			if float(strs[2]) < float(time0[2]):
				output.write(strs[0].strip()+'\t'+strs[1].strip()+'\t'+strs[2].strip()+'\t'+'1\n')
			else:
				output.write(strs[0].strip() + '\t'+ strs[1].strip() + '\t'+strs[2].strip() + '\t'+ '0\n')
		elif (float(strs[0]) >= float(time1[0])) and (float(strs[0])<float(time2[0])):
			if float(strs[2]) < float(time1[2]):
				output.write(strs[0].strip()+'\t'+strs[1].strip()+'\t'+strs[2].strip()+'\t'+'1\n')
			else:
				output.write(strs[0].strip() + '\t'+ strs[1].strip() + '\t'+strs[2].strip() + '\t'+ '0\n')
		elif (float(strs[0]) >= float(time2[0])) and (float(strs[0])<float(time3[0])):
		
			if float(strs[2]) < float(time2[2]):
				output.write(strs[0].strip()+'\t'+strs[1].strip()+'\t'+strs[2].strip()+'\t'+'1\n')
			else:
				output.write(strs[0].strip() + '\t'+ strs[1].strip() + '\t'+strs[2].strip() + '\t'+ '0\n')
		elif (float(strs[0]) >= float(time3[0])) and (float(strs[0])<float(time4[0])):
		
			if float(strs[2]) < float(time3[2]):
				output.write(strs[0].strip()+'\t'+strs[1].strip()+'\t'+strs[2].strip()+'\t'+'1\n')
			else:
				output.write(strs[0].strip() + '\t'+ strs[1].strip() + '\t'+strs[2].strip() + '\t'+ '0\n')
		elif (float(strs[0]) >= float(time4[0])) and (float(strs[0])<float(time5[0])):
		
			if float(strs[2]) < float(time4[2]):
				output.write(strs[0].strip()+'\t'+strs[1].strip()+'\t'+strs[2].strip()+'\t'+'1\n')
			else:
				output.write(strs[0].strip() + '\t'+ strs[1].strip() + '\t'+strs[2].strip() + '\t'+ '0\n')	
		elif (float(strs[0]) >= float(time5[0])) and (float(strs[0])<float(time6[0])):
		
			if float(strs[2]) < float(time5[2]):
				output.write(strs[0].strip()+'\t'+strs[1].strip()+'\t'+strs[2].strip()+'\t'+'1\n')
			else:
				output.write(strs[0].strip() + '\t'+ strs[1].strip() + '\t'+strs[2].strip() + '\t'+ '0\n')
		elif (float(strs[0]) >= float(time6[0])) and (float(strs[0])<float(time7[0])):
		
			if float(strs[2]) < float(time6[2]):
				output.write(strs[0].strip()+'\t'+strs[1].strip()+'\t'+strs[2].strip()+'\t'+'1\n')
			else:
				output.write(strs[0].strip() + '\t'+ strs[1].strip() + '\t'+strs[2].strip() + '\t'+ '0\n')							
		elif float(strs[0]) >= float(time7[0]):
		
			if float(strs[2]) < float(time7[2]):
				output.write(strs[0].strip()+'\t'+strs[1].strip()+'\t'+strs[2].strip()+'\t'+'1\n')
			else:
				output.write(strs[0].strip() + '\t'+ strs[1].strip() + '\t'+strs[2].strip() + '\t'+ '0\n')								
				

