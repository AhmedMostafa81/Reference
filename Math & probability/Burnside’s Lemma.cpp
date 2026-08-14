Necklace Coloring with Burnside
There are n positions and k colors.

Without considering rotation:
k^n
Consider all n rotations.
A rotation by r positions creates:
gcd(n,r) cycles.
For the coloring to remain unchanged, all positions in the same cycle must have the same color.
Therefore, the number of colorings fixed by rotation r is: k^gcd(n,r)

By Burnside's Lemma:

int sum = 0 ;
for(int r = 0 ; r <= n-1 ; r++)
  sum += k^gcd(n,r)
Answer= (1/n) * sum

Key idea: For each rotation, count its cycles; each cycle can independently choose one of the k colors.
