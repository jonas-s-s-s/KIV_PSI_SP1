# KIV_PSI_SP1
 Simple HTTP server 

root@psi-base-node-12:~# g++ --version
g++ (Ubuntu 9.4.0-1ubuntu1~20.04.2) 9.4.0

## Odůvodnění Obsahu tabulky
* Jedna z podmínek je:
> Zobrazujte jen „použitelné“ záznamy, na základě kterých je prováděn forwarding paketů"
* To znamená že z tabuky musíme vyřadit všechny záznamy, co mají ve sloupci `Gateway` adresu `00000000`, nýbrž tyto záznamy se nepoužívají pro "Forwarding packetů", ale jen pro odeslání rámce cílovému zařízení na L2 co se nachází v síti, která je přímo připojená na daný interface
* Forwarding samotný se provádí i na L2 ("frame forwarding"), zde jde ale o L2 PDU "frame", tedy "rámec" a ne "packet"
* https://www.geeksforgeeks.org/frame-forwarding-methods-on-switches/