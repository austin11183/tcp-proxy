## Phase1 建立一個 socket
建立      命名     連結                傳輸    停止
socket -> bind -> listen -> accept -> read -> close

socket: 開啟一個 socket，並回應一個檔案描述子
bind: 表示連結到傳輸提供者的位址上
listen: 將 socket 進入聆聽狀態
accept: 等待遠端連線訊號到達，並接受連線
read: 透過連線接收訊息
close: 停止 socket 工作

# socket 功能:
```
#include<sys/types.h>
#include<sys/socket.h>

int domain;
int type;
int protocol;
int fd;

fd = socket(domain, type, protocol);
```
socket() 如同一般檔案系統 open()系統呼叫一樣，會回整數(fd 又稱檔案描述子)，表示該 socket 識別值，檔案描述子大於 0 表示正常開起，並針對描述子的動作做處理(connect()、read())。也可以呼叫多個 socket()，來產生多個通訊端點，以不同的檔案描述子來區分。

# socket 參數:
(A) domain/family:
    又稱為family，是用來選擇哪一種通訊協定
    - AF_UNIX:此通訊家族並不是真正的網路通訊協定，而是用來作
      Unix 作業系統中，各程序（Process）之間的通訊使用。一般使用在回授（Loopback）傳
      輸提供者，而其應用在主機內程序之間的通訊。
    
    - AF_INET:
      Internet Protocol，此為 TCP/IP 的 Internet 通訊協定，傳輸提供者可能是 TCP 或是 UDP。
    
    - AF_NS: Xerox公司發展的通訊協定。

    - AF_IMPLINK: Interface Message Protocol。點到點連接，一般使用在租用電話線路。

    - AF 代表 Address Family 或是稱為 PF 又稱 Protocol Family。

(B) type 參數:
    參數 type 是該 socket 的類型，如下:
    - SOCK_STREAM: Stream Socket，傳輸提供者提供一個虛擬電路服務( TCP )。

    - SOCK_DGRAM: Datagram Socket。提供電報傳輸服務 ( UDP )。

    - SOCK_RAW: Raw Socket。通訊協定型態在傳輸層之下，譬如在 AF_INET（傳輸層為
      TCP 或 UDP）模式，SOCK_RAW 的通訊協定可以是 IP（Internet Protocol）或 ICMP
     （Internet Control Message Protocol）。

    - SOCK_SEQPACKET: Sequenced Packet Socket。提供虛擬電路（TCP）並附有維護訊息的功能。

(C) protocol 參數:
    - 此參數是在某一個 domain/family 之下，選擇要哪一種協定，例如選定 AF_INET family 系列時，所使用協定TCP、UDP、IP大多數使用的協定已經指定完成，因此 protocol 的值都是設定為 0，當socket 是 SOCK_RAW 時，必須指定他的上層協定是TCP、UDP、IP、ICMP。
    - socketpair() 可以開啟兩個 Socket ，且必須在同一個系統上，所以 domain 只能設定 AF_UNIX，否則會發生錯誤
    ```
    #include<sys/types.h>
    #include<sys/socket.h>

    int domain, type, protocol, status, fd_array[2];
    
    status = socketpair(domain, type, protocol, fd_array);
    ```
    系統正常的話，status 會回 0，而 fd_array 回應兩個檔案描述子來代表 socket.