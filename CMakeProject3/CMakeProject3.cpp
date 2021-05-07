// CMakeProject3.cpp : Defines the entry point for the application.
//

#include "CMakeProject3.h"
#include <unordered_map>
#include <algorithm>
using namespace std;
unordered_map<int, double> hikerSpeedMap;
#define TOTAL_HIKERS 22

void addHikerSpeedMap(int hiker, double speed) {
	hikerSpeedMap[hiker] = speed;
}
void calCurBridgeTimes(vector<double>& curBridgeHikerTime, int bridgeLen) {
	for (auto& it : hikerSpeedMap) {
		curBridgeHikerTime.push_back(bridgeLen / it.second);
	}
	sort(curBridgeHikerTime.begin(), curBridgeHikerTime.end());
}
int countRemPeople(int mask) {
	int cnt = 0;
	while (mask) {
		mask &= (mask - 1);
		cnt++;
	}
	return cnt;
}
double calMinTimeCurHikers(vector<double>& curBridgeHikerTime, bool sideIndicator, int &endIdx, int mask, vector<vector<double>> &hikerMemo) {
	if (mask == 0)
		return 0;
	if (hikerMemo[mask][sideIndicator] != -1)
		return hikerMemo[mask][sideIndicator];
	double retVal = DBL_MAX;
	int endmask = (1 << endIdx);
	endmask--;
	endmask ^= mask;
	if (sideIndicator) {
		double minHikerVal = DBL_MAX;
		int hikerIdx = 0;
		for (int i = 0; i < endIdx; ++i) {
			if (endmask & (1 << i)) {
				if (minHikerVal > curBridgeHikerTime[i]) {
					hikerIdx = i;
					minHikerVal = curBridgeHikerTime[i];
				}
			}
		}
		
		retVal = curBridgeHikerTime[hikerIdx] + calMinTimeCurHikers(curBridgeHikerTime, !sideIndicator, endIdx, mask | (1 << hikerIdx), hikerMemo);
	}
	else {
		int numPeopleRem = countRemPeople(mask);
		if (numPeopleRem > 1) {
			for (int i = 0; i < endIdx; ++i) {
				if (!(mask & (1 << i)))
					continue;
				for (int j = i + 1; j < endIdx; ++j) {
					if (mask & (1 << j)) {
						double maxVal = max(curBridgeHikerTime[i], curBridgeHikerTime[j]);
						int ival = 1 << i;
						int jval = 1 << j;
						int maskNewVal = ((mask) ^ ival) ^ jval;
						maxVal += calMinTimeCurHikers(curBridgeHikerTime, !sideIndicator, endIdx, mask ^ (1 << i) ^(1<<j), hikerMemo);
						retVal = min(maxVal, retVal);
					}
				}
			}
		} else {
			for (int i = 0; i < endIdx; i++) {
				if (mask & (1 << i)) {
					retVal = curBridgeHikerTime[i];
					break;
				}
			}
		}
		
		
	}
	hikerMemo[mask][sideIndicator] = retVal;
	return retVal;
}
double calMinTime(vector<double>& curBridgeHikerTime, vector<vector<double>> &hikerMemo) {
	int totHikers = curBridgeHikerTime.size();
	int mask = (1 << totHikers);
	mask--;
	double retval = calMinTimeCurHikers(curBridgeHikerTime, false, totHikers, mask, hikerMemo);
	return retval;
}
int main()
{
	double retVal = 0;
	vector<double> curBridgeHikerTime;
	vector<vector<double>> hikerMemo(TOTAL_HIKERS, vector<double>(2, -1));
	addHikerSpeedMap(1, 100);
	addHikerSpeedMap(2, 50);
	addHikerSpeedMap(3, 20);
	addHikerSpeedMap(4, 10);

	calCurBridgeTimes(curBridgeHikerTime, 100);
	
	retVal = calMinTime(curBridgeHikerTime, hikerMemo);
	cout << retVal << endl;
}
