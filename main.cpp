#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;
    struct Item { int p, w, v; };
    vector<Item> items(n);
    for (int i = 0; i < n; ++i) {
        cin >> items[i].p >> items[i].w >> items[i].v;
    }

    // Bounded knapsack with monotone queue optimization
    // Complexity: O(n * m)
    vector<long long> dp(m + 1, LLONG_MIN / 4);
    dp[0] = 0;

    for (const auto &it : items) {
        int w = it.w, v = it.v, p = it.p;
        if (w == 0) { // Zero-weight items: take all if value positive
            if (v > 0) {
                long long add = 1LL * p * v;
                for (int j = 0; j <= m; ++j) {
                    if (dp[j] > LLONG_MIN / 8) dp[j] += add;
                }
            }
            continue;
        }
        // Process residues modulo w
        for (int r = 0; r < w && r <= m; ++r) {
            deque<pair<int,long long>> dq; // pair of index t and value = dp[r + t*w] - t*v
            // t ranges so that idx = r + t*w <= m
            for (int t = 0; r + t * w <= m; ++t) {
                int idx = r + t * w;
                long long val = dp[idx] - 1LL * t * v;
                // maintain window size p (we can use at most p items): t - front.t <= p
                while (!dq.empty() && dq.front().first < t - p) dq.pop_front();
                while (!dq.empty() && dq.back().second <= val) dq.pop_back();
                dq.emplace_back(t, val);
                dp[idx] = dq.front().second + 1LL * t * v; // best using up to p items
            }
        }
    }

    long long ans = 0;
    for (int j = 0; j <= m; ++j) ans = max(ans, dp[j]);
    cout << ans << '\n';
    return 0;
}
