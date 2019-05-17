#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
class Solution {
public:
	string shortestSuperstring(vector<string>& A) {
		const int n = A.size();
		vector<vector<int>> dis(n, vector<int>(n, 0));
		for (int i = 0; i < n; ++i) {
			for (int j = i + 1; j < n; ++j) {
				if (i == j) continue;
				dis[i][j] = GetDis(A[i], A[j]);
				dis[j][i] = GetDis(A[j], A[i]);
			}
		}
		int target = (1 << n) - 1;
		vector<vector<int>> dp(n, vector<int>(target + 1, INT_MAX)), path(n, vector<int>(target + 1, -1));
		for (int i = 0; i < n; ++i) dp[i][1 << i] = A[i].length();
		for (int state = 1; state <= target; ++state) {
			for (int i = 0; i < n; ++i) {
				if (!(state & 1 << i)) continue;
				for (int j = 0; j < n; ++j) {
					if (!(state & 1 << j)) continue;
					int prev_state = state & ~(1 << i);
					if (dp[j][prev_state] != INT_MAX && dp[j][prev_state] + dis[j][i] < dp[i][state]) {
						path[i][state] = j;
						dp[i][state] = dp[j][prev_state] + dis[j][i];
					}
				}
			}
		}
		int min_total_dis = INT_MAX;
		int min_path_end = -1;
		for (int i = 0; i < n; ++i) {
			if (dp[i][target] < min_total_dis) {
				min_total_dis = dp[i][target];
				min_path_end = i;
			}
		}
		vector<int> final_path;
		final_path.reserve(n);
		int end_state = target;
		int cur_node = min_path_end;
		while (end_state != 0) {
			final_path.emplace_back(cur_node);
			int prev_node = path[cur_node][end_state];
			end_state &= ~(1 << cur_node);
			cur_node = prev_node;
		} 
		cur_node = final_path.back(); 
		string res = A[cur_node];
		for (int i = n - 2; i >= 0; --i) {
			int next_node = final_path[i];
			const string& next_str = A[next_node];
			int cur_dis = dis[cur_node][next_node]; 
			res += next_str.substr(next_str.length() - cur_dis);
			cur_node = next_node;
		}
		return res;
	}

	int GetDis(const string& s1, const string& s2) {
		const int len1 = s1.length(), len2 = s2.length();
		int min_len = min(len1, len2);
		int overlap = min_len;
		for (; overlap > 0; --overlap) {
			if (s2.substr(0, overlap) == s1.substr(len1 - overlap)) return len2 - overlap;
		}
		return len2;
	}
};

int main() {
	Solution sol;
	vector<string> svec = { "catg","ctaagt","gcta","ttca","atgcatc" };
	auto res = sol.shortestSuperstring(svec);
	cout << res << endl;
	system("PAUSE");
	return 0;
}