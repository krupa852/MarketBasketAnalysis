# AprioriAlgo

Run commands:<br />

g++ -std=c++14 AprioriAlgorithm.cpp<br />
./a.out [filename] [support] [confidence]<br />

Ex:<br />
./a.out Testset2.txt 3 0.5<br />

Output:<br />

Bread: 5<br />
Diapers: 6<br />
Butter: 5<br />
Cookies: 4<br />
Beer: 4<br />
Milk: 5<br />
Bread, Diapers: 3<br />
Bread, Butter: 5<br />
Bread, Milk: 3<br />
Diapers, Milk: 4<br />
Butter, Diapers: 3<br />
Butter, Milk: 3<br />
Bread, Butter, Diapers: 3<br />
Bread, Butter, Milk: 3<br />

The number of association rules: 24<br />
Diapers -> Bread [support = 3, confidence = 50%]<br />
Bread -> Diapers [support = 3, confidence = 60%]<br />
Butter -> Bread [support = 5, confidence = 100%]<br />
Bread -> Butter [support = 5, confidence = 100%]<br />
Milk -> Bread [support = 3, confidence = 60%]<br />
Bread -> Milk [support = 3, confidence = 60%]<br />
Milk -> Diapers [support = 4, confidence = 80%]<br />
Diapers -> Milk [support = 4, confidence = 66.6667%]<br />
Diapers -> Butter [support = 3, confidence = 50%]<br />
Butter -> Diapers [support = 3, confidence = 60%]<br />
Milk -> Butter [support = 3, confidence = 60%]<br />
Butter -> Milk [support = 3, confidence = 60%]<br />
Diapers -> Bread, Butter [support = 3, confidence = 50%]<br />
Butter -> Bread, Diapers [support = 3, confidence = 60%]<br />
Butter, Diapers -> Bread [support = 3, confidence = 100%]<br />
Bread -> Butter, Diapers [support = 3, confidence = 60%]<br />
Bread, Diapers -> Butter [support = 3, confidence = 100%]<br />
Bread, Butter -> Diapers [support = 3, confidence = 60%]<br />
Milk -> Bread, Butter [support = 3, confidence = 60%]<br />
Butter -> Bread, Milk [support = 3, confidence = 60%]<br />
Butter, Milk -> Bread [support = 3, confidence = 100%]<br />
Bread -> Butter, Milk [support = 3, confidence = 60%]<br />
Bread, Milk -> Butter [support = 3, confidence = 100%]<br />
Bread, Butter -> Milk [support = 3, confidence = 60%]<br />
