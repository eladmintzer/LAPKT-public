(define (problem BLOCKS-11-1)
(:domain BLOCKS)
(:objects B C E A H K I G D F J - block)
(:INIT (CLEAR J) (CLEAR F) (CLEAR D) (CLEAR G) (ONTABLE I) (ONTABLE K)
 (ONTABLE H) (ONTABLE A) (ON J I) (ON F E) (ON E K) (ON D C) (ON C H) (ON G B)
 (ON B A) (HANDEMPTY))
(:goal (AND (ON B D) (ON D J) (ON J K) (ON K H) (ON H A) (ON A C) (ON C F)
            (ON F G) (ON G I) (ON I E)))
)
