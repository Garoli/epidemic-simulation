set title "Simulation results";
set xlabel "Round number";
set ylabel "People number";

plot 'evolution.txt' u 1 w l title 'Healthy people',\
 'evolution.txt' u 2 w l title 'Sick people', \
 'evolution.txt' u 3 w l title 'Dead people',\
 'evolution.txt' u 4 w l title 'Burnt people';