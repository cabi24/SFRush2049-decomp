# Sound Effect Catalog

Sources: `reference/repos/rushtherock/game/sounds.h`, `reference/repos/rushtherock/game/sounds.c`, `reference/repos/rushtherock/game/carsnd.c`.

## Engine Sounds
| ID | Name | Description | Arcade Ref |
|----|------|-------------|------------|
| 0x8022 | S_ENGINE_STOP | Stop the listener's engine | sounds.h:30 |
| 0x8023 | S_ENGINE_CHANGE_PF | Change pitch/filter engine | sounds.h:31 |
| 0x8024 | S_ENGINE_CHANGE_PFV | Change pitch/filter/volume engine | sounds.h:32 |
| 0x8025 | S_ENGINE_CHANGE_AUTO | Change speed of automatic engine | sounds.h:33 |
| 0x8026 | S_ENGINE_CRASH | Quick change of automatic engine | sounds.h:34 |
| 0x8030 | S_MULTIDUMMY | Doppler multi-idle to match moob engine | sounds.h:37 |
| 0x8031 | S_M_MULTIIDLE | Multi-idle for other car engine (rpm/load) | sounds.h:38 |
| 0x8032 | S_MULTIIDLE | Multi-idle for own engine (rpm/load) | sounds.h:39 |
| 0x8033 | S_AUDREY | APM engine Audrey (rpm/load) | sounds.h:40 |
| 0x8034 | S_NSX | APM engine NSX (rpm/load) | sounds.h:41 |
| 0x8035 | S_GHR | APM engine HiLoad (rpm/load) | sounds.h:42 |
| 0x8036 | S_TPM | APM engine TPM (rpm/load) | sounds.h:43 |
| 0x8086 | S_BOG | Engine bogging | sounds.h:132 |
| 0x8087 | S_KBOG | Kill engine bogging | sounds.h:133 |
| 0x809A | S_SPUTTER | Engine sputters at timeout | sounds.h:158 |
| 0x809B | S_KSPUTTER | Kill engine sputter | sounds.h:160 |

## Collision Sounds
| ID | Name | Description | Arcade Ref |
|----|------|-------------|------------|
| 0x8075 | S_CARBUMP | Small collision | sounds.h:121 |
| 0x8076 | S_CARSCRAPE | High speed scraping | sounds.h:122 |
| 0x8077 | S_CARSMASH | Medium collision | sounds.h:123 |
| 0x8078 | S_CURBWHUMP | Curb whump | sounds.h:124 |
| 0x8079 | S_BOTTOMOUT | Bottoming out | sounds.h:125 |
| 0x807A | S_CONES | Cones hit | sounds.h:126 |
| 0x807B | S_GLASS | Plate glass hit | sounds.h:127 |
| 0x807C | S_PMETER | Parking meter hit | sounds.h:128 |
| 0x807D | S_SCRAPELOOP | Looping scrape | sounds.h:129 |
| 0x807E | S_KSCRAPELOOP | Kill scrape loop | sounds.h:130 |
| 0x807F | S_LIGHTPOLE | Light pole hit | sounds.h:131 |
| 0x8080 | S_TREE | Tree hit | sounds.h:132 |
| 0x8081 | S_FENCE | Fence hit | sounds.h:133 |
| 0x8082 | S_BOOM | Heavy impact | sounds.h:134 |
| 0x80D2 | S_BUSH | Bush hit | sounds.h:203 |

## Tire/Road/Environment Noises
| ID | Name | Description | Arcade Ref |
|----|------|-------------|------------|
| 0x806C | S_SKID_CUSTOM1 | Skid sound (handle/pitch/vol) | sounds.h:108 |
| 0x806D | S_SKID_CUSTOM2 | Skid sound (handle/pitch/vol) | sounds.h:109 |
| 0x806E | S_WINDNOISE | Wind noise (starts ~30 mph) | sounds.h:110 |
| 0x806F | S_ROADNOISE | Road noise | sounds.h:111 |
| 0x8070 | S_GRAVELNOISE | Gravel noise | sounds.h:112 |
| 0x8071 | S_WATERROAD | Water road noise | sounds.h:113 |
| 0x8072 | S_PEELOUT | Tire peelout | sounds.h:114 |
| 0x8073 | S_SQUEAL_LEFT | Left tire squeal | sounds.h:115 |
| 0x8074 | S_SQUEAL_RIGHT | Right tire squeal | sounds.h:116 |
| 0x8083 | S_CAR_LANDS | Car landing after jump | sounds.h:136 |
| 0x8096 | S_SPLASH | Splash in water | sounds.h:152 |

## UI/Menu/Countdown
| ID | Name | Description | Arcade Ref |
|----|------|-------------|------------|
| 0x808A | S_TRACKBROWSE | Track browse sound | sounds.h:139 |
| 0x808B | S_TRANSELECT | Transmission select | sounds.h:141 |
| 0x808C | S_LIGHTSOUND | Car browse sound | sounds.h:142 |
| 0x808E | S_TURNTABLE | Car turntable loop | sounds.h:144 |
| 0x808F | S_KTURNTABLE | Kill turntable loop | sounds.h:145 |
| 0x8090 | S_COIN1 | Partial credit | sounds.h:146 |
| 0x8091 | S_COIN2 | Full credit | sounds.h:147 |
| 0x8092 | S_THREE | Countdown three | sounds.h:148 |
| 0x8093 | S_TWO | Countdown two | sounds.h:149 |
| 0x8094 | S_ONE | Countdown one | sounds.h:150 |
| 0x8095 | S_RUSH | Countdown rush | sounds.h:151 |
| 0x80B5 | S_BEEPCAR | Car select beep | sounds.h:182 |
| 0x80B6 | S_BEEPWARN1 | Beginner warning beep | sounds.h:183 |
| 0x80B7 | S_KBEEPWARN1 | Kill warning beep 1 | sounds.h:184 |
| 0x80B8 | S_BEEPWARN2 | Advanced warning beep | sounds.h:185 |
| 0x80B9 | S_KBEEPWARN2 | Kill warning beep 2 | sounds.h:186 |
| 0x80BA | S_BEEPWARN3 | Expert warning beep | sounds.h:187 |
| 0x80BB | S_KBEEPWARN3 | Kill warning beep 3 | sounds.h:188 |

## Voice Samples / Announcements
| ID | Name | Description | Arcade Ref |
|----|------|-------------|------------|
| 0x805F | S_ANN1 | Announcer 1 | sounds.h:79 |
| 0x8060 | S_ANN2 | Announcer 2 | sounds.h:80 |
| 0x8061 | S_ANN3 | Announcer 3 | sounds.h:81 |
| 0x8062 | S_ANN4 | Announcer 4 | sounds.h:82 |
| 0x8063 | S_ANN5 | Announcer 5 | sounds.h:83 |
| 0x8064 | S_ANN6 | Announcer 6 | sounds.h:84 |
| 0x8084 | S_CHKPNTSTATIC | "Checkpoint!" | sounds.h:137 |
| 0x8097 | S_DANGEROUS | "It's dangerous!" | sounds.h:153 |
| 0x809C | S_START1 | Starting announcer 1 | sounds.h:162 |
| 0x809E | S_START2 | Starting announcer 2 | sounds.h:164 |
| 0x80A0 | S_START3 | Starting announcer 3 | sounds.h:166 |
| 0x80A2 | S_FINALL | "Final lap" | sounds.h:168 |
| 0x80A3 | S_2L | "2 laps to go" | sounds.h:169 |
| 0x80A4 | S_3L | "3 laps to go" | sounds.h:170 |
| 0x80A5 | S_4L | "4 laps to go" | sounds.h:171 |
| 0x80A6 | S_5L | "5 laps to go" | sounds.h:172 |
| 0x80A7 | S_6L | "6 laps to go" | sounds.h:173 |
| 0x80A8 | S_7L | "7 laps to go" | sounds.h:174 |
| 0x80A9 | S_8L | "8 laps to go" | sounds.h:175 |
| 0x80AA | S_9L | "9 laps to go" | sounds.h:176 |
| 0x80C2 | S_8WAY | "8-way link" exclamation | sounds.h:194 |
| 0x80C3 | S_2CARS | "2 cars in a pod" | sounds.h:195 |
| 0x80C4 | S_3CARS | "3 cars in a pod" | sounds.h:196 |
| 0x80C5 | S_4CARS | "4 cars in a pod" | sounds.h:197 |
| 0x80C6 | S_5CARS | "5 cars in a pod" | sounds.h:198 |
| 0x80C7 | S_6CARS | "6 cars in a pod" | sounds.h:199 |
| 0x80C8 | S_7CARS | "7 cars in a pod" | sounds.h:200 |
| 0x80C9 | S_8CARS | "8 cars in a pod" | sounds.h:201 |
| 0x80D1 | S_LINKED | "Linked" exclamation | sounds.h:202 |
| 0x80D3 | S_RUSHWHISPER | Whisper encouragement | sounds.h:204 |
| 0x80EE | S_YELL | Yell exclamation | sounds.h:221 |
| 0x80EF | S_ITSFAT | "It's fat" exclamation | sounds.h:222 |
| 0x80F0 | S_CHIPS | "All that and a bag o' chips" | sounds.h:223 |
| 0x80F1 | S_YOUGOTTA | "I can't do anymore..." | sounds.h:224 |
| 0x80F2 | S_ITSINSANE | "It's insane" exclamation | sounds.h:225 |
| 0x80F3 | S_LOOKOUT | "Look out" exclamation | sounds.h:226 |
| 0x80F4 | S_BADLICIOUS | "Badlicious" exclamation | sounds.h:227 |

## Ambient / Music / Radio
| ID | Name | Description | Arcade Ref |
|----|------|-------------|------------|
| 0x803C | S_WINNER | Winner tune | sounds.h:55 |
| 0x803D | S_KWINNER | Kill winner tune | sounds.h:56 |
| 0x803E | S_SEAGULLS | Seagulls ambient | sounds.h:57 |
| 0x803F | S_KSEAGULLS | Kill seagulls | sounds.h:58 |
| 0x8040 | S_EXPLO | Explo tune | sounds.h:59 |
| 0x8041 | S_KEXPLO | Kill explo tune | sounds.h:60 |
| 0x8042 | S_WHATSYERNAME | Music tag | sounds.h:61 |
| 0x8043 | S_KWHATSYERNAME | Kill music tag | sounds.h:62 |
| 0x8044 | S_PARKBIRD | Park bird ambient | sounds.h:63 |
| 0x8045 | S_KPARKBIRD | Kill park bird | sounds.h:64 |
| 0x8046 | S_SELECT | Car select music | sounds.h:65 |
| 0x8047 | S_KSELECT | Kill select music | sounds.h:66 |
| 0x8048 | S_FANFARE2 | "3-2-1" ascend tune | sounds.h:67 |
| 0x8049 | S_GARAGEAMB | Garage ambience | sounds.h:68 |
| 0x804A | S_KGARAGEAMB | Kill garage ambience | sounds.h:69 |
| 0x804B | S_CARSELECT | Car select tune | sounds.h:71 |
| 0x804C | S_KCARSELECT | Kill car select tune | sounds.h:72 |
| 0x804D | S_FOGHORN | Foghorn ambient | sounds.h:73 |
| 0x804E | S_KFOGHORN | Kill foghorn | sounds.h:74 |
| 0x8050 | S_AUG8ATT | Attract music (AUG8) | sounds.h:76 |
| 0x8051 | S_KAUG8ATT | Kill AUG8 attract | sounds.h:77 |
| 0x8052 | S_PULPATT | Attract music (Pulp) | sounds.h:78 |
| 0x8053 | S_KPULPATT | Kill Pulp attract | sounds.h:79 |
| 0x8054 | S_STLATT | Attract music (STL) | sounds.h:80 |
| 0x8055 | S_KSTLATT | Kill STL attract | sounds.h:81 |
| 0x8056 | S_ZETHNOATT | Attract music (Zethno) | sounds.h:82 |
| 0x8057 | S_KZETHNOATT | Kill Zethno attract | sounds.h:83 |
| 0x8058 | S_RAVERUSHATT | Attract music (Raverush) | sounds.h:84 |
| 0x8059 | S_KRAVERUSHATT | Kill Raverush attract | sounds.h:85 |
| 0x805A | S_BLUESATT | Attract music (Blues) | sounds.h:86 |
| 0x805B | S_KBLUESATT | Kill Blues attract | sounds.h:87 |
| 0x80D4 | S_CROWDRIGHT | Crowd right ambient | sounds.h:205 |
| 0x80D5 | S_KCROWDRIGHT | Kill crowd right | sounds.h:206 |
| 0x80D6 | S_CROWDLEFT | Crowd left ambient | sounds.h:207 |
| 0x80D7 | S_KCROWDLEFT | Kill crowd left | sounds.h:208 |
| 0x80D8 | S_THEROCK | "The Rock" tune | sounds.h:210 |
| 0x80D9 | S_KTHEROCK | Kill "The Rock" tune | sounds.h:211 |
| 0x80DA | S_PRISONSIREN | Prison siren ambient | sounds.h:212 |
| 0x80DB | S_KPRISONSIREN | Kill prison siren | sounds.h:213 |
| 0x80F5 | S_WAVENET | Wavenet tune | sounds.h:228 |
| 0x80F6 | S_KWAVENET | Kill wavenet | sounds.h:229 |
| 0x80F7 | S_WAVETAG1 | Wavenet tag 1 | sounds.h:230 |
| 0x80F8 | S_WAVETAG2 | Wavenet tag 2 | sounds.h:231 |
| 0x80F9 | S_TOURNEY1 | Tournament promo | sounds.h:232 |
| 0x80FA | S_REGIONAL | Regional winner promo | sounds.h:233 |
| 0x80FB | S_KREGIONAL | Kill regional promo | sounds.h:234 |
| 0x80FC | S_REGISTER1 | "Register to win" | sounds.h:235 |
| 0x80FD | S_JAILDOORLOOP | Jaildoor loop | sounds.h:236 |
| 0x80FE | S_KJAILDOORLOOP | Kill jaildoor loop | sounds.h:237 |
| 0x80FF | S_JAILDOORCLANG | Jaildoor clang | sounds.h:238 |

## N64 Sound Call Analysis
| game.c Line | Function | ID Used | Likely Sound |
|-------------|----------|---------|--------------|
| src/game/game.c:13355 | voice_play | 6 | S_FINALL ("Final lap") |
| src/game/game.c:13358 | voice_play | 8 | S_CHKPNTSTATIC ("Checkpoint!") |
| src/game/game.c:13514 | voice_play | 3 | Likely announcer placement (S_ANN1/S_ANN2/S_ANN3) |
| src/game/game.c:13598 | voice_play | 4 | Likely announcer placement (S_ANN2/S_ANN3) |
| src/game/game.c:13600 | voice_play | 5 | Likely announcer placement (S_ANN3/S_ANN4) |
| src/game/game.c:25696 | sound_play | 2 | UI tick (likely S_TRACKBROWSE or S_LIGHTSOUND) |
| src/game/game.c:25753 | sound_play | 3 | UI open (likely S_CARSELECT or S_SELECT) |
| src/game/game.c:25755 | sound_play | 4 | UI close (likely S_KCARSELECT or S_KSELECT) |
| src/game/game.c:26260 | sound_play | 40 | Coin sound (S_COIN1/S_COIN2) |
| src/game/game.c:26655 | sound_play | 30 | Countdown beep (S_THREE/S_TWO/S_ONE/RUSH) |

## Sound Trigger Conditions
- Engine audio uses APM engine IDs (`S_AUDREY`, `S_NSX`, `S_GHR`, `S_TPM`) with rpm/load parameters; see `sounds.c` own_engine table.
- Wind noise starts near 30 mph and uses pitch/volume controls (`S_WINDNOISE` comment in `sounds.h`).
- Road, gravel, and water noises are continuous loops with pitch/volume scaling (`S_ROADNOISE`, `S_GRAVELNOISE`, `S_WATERROAD`).
- Skid and squeal sounds use per-wheel or per-axle states in `carsnd.c` (`scrch_state`, `skid_intensity`).
- Collision sounds (bump/smash/scrape) are tied to collision forces and scrape timing in `carsnd.c`.
- Ambient tunes and radio/attract stations are managed in `sounds.c` via station lists (`demo_radio_on/off`).
