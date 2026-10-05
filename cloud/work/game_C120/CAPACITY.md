# C120 semantic capacities

Both local arrays contain six consumed car-domain entries. Native index-list access uses signed16 entries; distance accesses use signed32 entries indexed by the real selected car index. No capacity follows solely from stack offsets.

The original setup/initialization sources provide independent domain evidence: init_state_continue computes extra=min(6-D_8014A108,D_80142724), then D_801543CA=D_8014A108+extra; its fixed configuration loop covers six slots. func800EC190 explicitly sets D_801543CA=6. func800EC914 constructs D_80152744 from the subset of active entries in that car count, stores each real i index into its selected record, and completes initialization up to six. Thus pending count, selected car index and metric subscript all use the six-car domain. We use exactly these six-entry capacities, without adding speculative unused stack room. Native instructions will be independently audited before a positive claim.

Actual caller context is the complete B125 func800F8EC8_native body, unchanged. Its declarations-only record carriers are refined to expose actual observed rank_metric@256, completion@239, eligibility@857 and vehicle car_index@1990/kind@2024 without changing existing field offsets or body text. No accepted source or shared context is changed.
