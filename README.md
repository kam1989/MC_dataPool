# MC_dataPool
데이터를 다루는 풀. 커넥션 풀과 비슷하다. 
풀 자체에서 생성, 해지를 하지 않는다.

테스트 환경: 
cpu: amd Phenom(tm)2 X4 960T processor
ram: 8GB

테스트 데이터:
데이터가 풀에 들어갔다가 얼마나 빨리 나오느냐를 측정.
_Query_perf_counter() 로 값을 측정했다.

테스트 결과:
lockfree: 

		min		14	__int64
		max		1288917	__int64
		total	6225768304	__int64
		count	100000000	__int64

		min		14	__int64
		max		1646764	__int64
		total	6447566181	__int64
		count	100000000	__int64

		min		14	__int64
		max		1798104	__int64
		total	7037341871	__int64
		count	100000000	__int64

		min		14	__int64
		max		1143086	__int64
		total	6859316296	__int64
		count	100000000	__int64
		
		min		14	__int64
		max		1293180	__int64
		total	6859369020	__int64
		count	100000000	__int64
    나열된 일부 결과들처럼 68xxxxxxxx ~ 70xxxxxxxx 사이에서 측정됨.
    평균 68.x~70.x
    어마어마한 max 값은 확실한 원인을 찾지 못함. context switch 가 측정도중 일어났다고 추측.

boost + queue:

		min	21	__int64
		max	1494346	__int64
		total	8402033703	__int64
		count	100000000	__int64
		
		min		21	__int64
		max		1159725	__int64
		total	8139557440	__int64
		count	100000000	__int64
		
		min		21	__int64
		max		1399937	__int64
		total	8071228221	__int64
		count	100000000	__int64
		
		min		21	__int64
		max		1417689	__int64
		total	8027257329	__int64
		count	100000000	__int64
		
		min		21	__int64
		max		1564696	__int64
		total	8212560363	__int64
		count	100000000	__int64
		
		min		21	__int64
		max		1879402	__int64
		total	8018344026	__int64
		count	100000000	__int64

		min	21	__int64
		max	1806826	__int64
		total	8528277198	__int64
		count	100000000	__int64

    나열된 일부 결과들처럼 80xxxxxxxx ~ 85xxxxxxxx 사이에서 측정됨.
    평균 80.x~85.x

mutex + list:

    		min		14	__int64
		max		1150807	__int64
		total	12349768263	__int64
		count	100000000	__int64
		
				min	14	__int64
		max	1145480	__int64
		total	12134038126	__int64
		count	100000000	__int64

		min		14	__int64
		max		1002001	__int64
		total	12024300022	__int64
		count	100000000	__int64
    
    나열한 일부 결과들처럼 119xxxxxxxx ~ 120xxxxxxxx 사이에서 측정되며 측정별 시간차이가 거의 없다.
    평균 119.x ~ 120.x
    max 값의 범위가 lockfree 보다는 훨씬 안정적임. 10xxxxx ~ 11xxxxx 사이에서 측정.
