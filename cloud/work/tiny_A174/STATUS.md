# A174 direct AnimateShadow donor control: NONMATCH326/356

The original visuals.c:AnimateShadow uses xyz[4][3] and a single consumed len reused for each edge expansion. B94's frozen complete native reconstruction used a Vec3 array with flat float-pointer casts. This separately labeled donor control uses the genuine multidimensional array and original len expression/compound-assignment forms while preserving every N64-specific guard, four height differences, clamped working heights versus raw average, four corner ordering, asymmetrical scale parameters, alpha/selection flags and actual full geometry call.

No unused donor pos/mat arrays, extra local capacity, source padding, fictitious helper or frame reservation is added. Native local capacity remains exactly4x3 floats plus4 height floats. IDO's actual fabsf intrinsic contract and literal flags are retained.

Fresh O3 sourceSHA73825a0780951420ff3a964def8f7d8fdcf024ea08e3999087727f9b6db08e31 compiles326/356, with no excess/unresolved/unverified/errors, equal to the B94 intrinsic baseline. Thus the direct donor indexing/len contract does not improve the residual; claims stay empty and no further array/coloring controls follow. context_origin.json binds unchanged historical source/donor hashes. All original B94 files remain untouched. No shared production changes.
