# Egzamino-uzduotis

Ši programa skirta pateikto teksto analizavimui iš tekstinio dokumento (kode pateikiamas dokumentas Kaunas.txt).
Programa atlieka tris užduotis:
1. Suranda žodžius, kurie pasikartoja daugiau nei vieną kartą ir suskaičiuoja kiek kartų jie pasikartojo. Rezultatas išvedamas į dokumentą rezultatas.txt.
2. Sukuria cross-reference lentelę, kurioje pateikiamas žodis, jo pasikartojimų skaičius ir visų eilučių numeriai, kuriose buvo šis žodis. Rezultatas išvedamas į dokumentą cross_reference.txt
3. Suranda URL adresus ir juos išspausdina į dokumentą hyperlinkai.txt
<br />
Tekstiniame dokumente Kaunas.txt yra nukopijuotas Vikipedijos puslapis apie Kauną (https://lt.wikipedia.org/wiki/Kaunas).
Tekstiniame dokumente tld-list-basic.txt yra galimų domenų sąrašas, kuris yra naudojamas URL identifikavimui.
Pagrindinės funkcijos apima TLD sąrašo nuskaitymą, URL identifikavimą, netinkamų simbolių pašalinimą, žodžių pasikartojimo sakaičiavimą ir rezultatų įrašymą į atskirus dokumentus.
Programa naudoja std::string, ir asociatyvius konteinerius.
<br />
<br />
Sukurtų dokumentų pavyzdys:
<br />
<img width="649" alt="image" src="https://github.com/miglmigl/Egzamino-uzduotis/assets/144427943/76ff4575-d531-4a8a-9c0f-26d780008817">
<img width="652" alt="image" src="https://github.com/miglmigl/Egzamino-uzduotis/assets/144427943/465c79ef-1cf7-4dd5-8eef-cda74249dfa6">
<img width="649" alt="image" src="https://github.com/miglmigl/Egzamino-uzduotis/assets/144427943/c83cc953-7ce0-47eb-8db8-81b35c2a98ad">


# Papildoma užduotis

Reikėjo surasti žodžius, kuriuose yra "aik".
<br />
Šiam tikslui buvo sukurta funkcija raskAik, kuri tokius žodžius deda į set'ą ir po iškvietimo išveda į dokumentą SuAIK.txt.
<br />
Dokumneto SuAIK.txt pavyzdys:
<br />
<img width="596" alt="image" src="https://github.com/miglmigl/Egzamino-uzduotis/assets/144427943/a5b0a01c-02b6-4e19-b9a2-8db19b844e78">

