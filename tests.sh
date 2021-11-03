#!/bin/bash

for ((i=1;i<=10;i++))
do
triang_intersections=$(./triangles < ./Tests/test$i/test)
answ=$(cat ./Tests/test$i/answ)
if [ "$answ" = "$triang_intersections" ]
then
echo "Test $i: Done"
else
echo "Test $i: Failed"
#echo "Intersections of Triangles:$(\n) $triang_intersections"
#echo "------------------------"
#echo  "Answer:$(\n) $answ"
fi
done