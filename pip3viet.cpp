// PIP3 - Personal Identity Profile Version 3
// Created by Michael Robbins and Rick Good
// PC program created 10/20/2018 - 4/5/2021 by Walter D. Pullen (Astara@msn.com)
// See also: https://sevenray.org/education/

#include <windows.h>
#include <shellapi.h>
#include <stdio.h>

// Definitions

#define szDate "Ngay 06 thang Tu nam 2021"
#define szAddr "Astara@msn.com"
#define szSave "pip3.sav"
#define fFalse 0
#define fTrue 1
#define cRay 7
#define cPart 4
#define cInit 27
#define cStatement 141
#define cAspire 14
#define cAspire2 13
#define cTrait 28
#define cTrait2 5
#define cResponse 7
#define colMax 80

#define cchSzMax (255*2)
#define chNull '\0'
#define FOdd(n) ((n) & 1)
#define NAbs(b) abs(n)
#define ChHex(n) ((char)((n) < 10 ? '0'+(n) : 'a'+(n)-10))
#define Rgb(bR, bG, bB) \
  (((long)(bR)) | ((long)(bG)<<8) | ((long)(bB)<<16))
#define RgbR(l) ((int)((long)(l) & 255))
#define RgbG(l) ((int)(((long)(l) >> 8) & 255))
#define RgbB(l) ((int)((long)(l) >> 16))
#define ClearB(pb, cb) memset(pb, 0, cb)

enum _entrycode {
  ecNormal = 0,
  ecRetry = 1,
  ecNew = 2,
  ecDone = 3,
  ecExit = 4,
  ecLast = 5,
  ecUndo = 6,
  ecGoto = 7,
  ecPart = 8,
  ecLate = 9,
};


// Types

typedef int flag;
typedef float real;

typedef struct _statement {
  int ray;
  int nType;
  flag fReverse;
  flag fSkip;
  char *sz;
} STATEMENT;

typedef struct _trait {
  int ray;
  char *sz;
} TRAIT;

typedef struct _response {
  int res;
  int resf;
} RESPONSE;

typedef struct _powerray {
  int typea;
  int typeb;
  int gen;
  int tot;
} POWRAY;


// Tables

const char *rgInit[cInit] = {
"day du ho ten",
"cho o hien tai cua ban (thanh pho, tinh, quoc gia)",
"noi sinh (thanh pho, tinh, quoc gia)",
"ngay sinh (thang, ngay, nam, phut)",
"nghe nghiep hien nay",
"nghe nghiep truoc day",
"chuyen nganh hoc va cac mon hoc yeu thich",
"so thich va thien huong",
"cac muc tieu lon trong cuoc song",
"cac nhan to dan ban den viec tim hieu huyen bi hoc",
"thoi gian ban da tim hieu ve huyen bi hoc",
"thoi gian ban da nghien cuu ve Alice Bailey",
"thoi gian ban da nghien cuu ve 7 Cung",
"thoi gian ban da nghien cuu ve Chiem tinh hoc Ngoai mon",
"thoi gian ban da nghien cuu ve Chiem tinh hoc Ngoai mon",
"Cung Chan Than uoc doan cua ban (1-7)",
"Muc do tu tin vao uoc doan Cung Chan than cua ban",
"Cung Linh hon uoc doan cua ban (1-7)",
"Muc do tu tin cua ban vao Cung Linh hon uoc doan",
"Cung Pham nga uoc doan cua ban (1-7)",
"Muc do tu tin cua ban vao Cung Pham nga uoc doan",
"Cung The tri uoc doan cua ban (1-7)",
"Muc do tu tin cua ban vao Cung The tri uoc doan",
"Cung the cam duc uoc doan cua ban (1-7)",
"Muc do tu tin cua ban vao Cung the cam duc uoc doan",
"Cung co the vat ly uoc doan cua ban (1-7)",
"Muc do tu tin cua ban vao Cung co the vat ly uoc doan",
};

const char *rgInit2[cInit] = {
"Ho Ten",
"Noi o hien tai",
"Noi sinh",
"Thoi gian sinh",
"Nghe nghiep hien nay",
"Nghe nghiep truoc day",
"Nhung chu de ua thich",
"Thien huong",
"Cac muc tieu lon trong cuoc song",
"Cac nhan to dan ban den viec tim hieu huyen bi hoc",
"Nghien cuu huyen bi hoc trong bao lau",
"Nghien cuu Alice Bailey trong bao lau",
"Nghien cuu Bay Cung trong bao lau",
"Nghien cuu Chiem tinh ngoai mon trong bao lau",
"Nghien cuu Chiem tinh hoc Noi mon trong bao lau",
"Cung Chan Than uoc doan",
"Muc do tu tin vao uoc doan Cung Chan than",
"Cung Linh hon uoc doan",
"Muc do tu tin cua ban vao Cung Linh hon",
"Cung Pham nga uoc doan",
"Muc do tu tin vao Cung Pham nga uoc doan",
"Cung The tri uoc doan",
"Muc do tu tin vao Cung The tri uoc doan",
"Cung the cam duc uoc doan",
"Muc do tu tin vao Cung the cam duc uoc doan",
"Cung co the vat ly uoc doan",
"Muc do tu tin vao Cung co the vat ly uoc doan",
};

const char *rgInitDef[cInit] = {
"HO THANH HOA",
"HANOI, HANOI, VIETNAM",
"HANOI, HANOI, VIETNAM",
"Thang 11 01, 1971 LUC 11:00pm",
"BAO MAU TAI GIA",
"QUAN LY IT",
"CONG NGHE TIN HOC",
"Huyen bi hoc",
"TRI LIEU HUYEN MON",
"TINH YEU VOI CHAN LY",
"Tu 2016",
"Tu 2018",
"Since 2018",
"Since 2018",
"Since 2018",
"1",
"THUONG CHAC CHAN",
"2",
"THUONG CHAC CHAN",
"1",
"THUONG CHAC CHAN",
"4 - cung 5 phu",
"THUONG CHAC CHAN",
"6",
"THUONG CHAC CHAN",
"3",
"THUONG CHAC CHAN",
};

#define G 1
#define a 2
#define b 4
#define Ga (G|a)
#define Gb (G|b)
#define norm 0
#define REVERSE 1
#define inc 0
#define SKIP 1

const STATEMENT rgStatement[cStatement] = {
{3, a, norm, inc, "Toi la mot nguoi co tu duy truu tuong dieu luyen, nguoi ma mot cach tu nhien co the nhan thay cac kieu mau truu tuong trong triet hoc, toan hoc, ly thuyet hoac cac moi quan he co tinh he thong hap dan hon rat nhieu viec kham pha va ap dung nhung kien thuc thuc tien, cu the, co tinh nghe thuat hay tham chi la khoa hoc, cho du nhung kien thuc cu the nhu vay von di ly thu va huu dung ngay lap tuc."},
{6, Gb, norm, inc, "Neu toi thuc su yeu ai do, toi se di den \"tan cung troi cuoi dat\" de bay to tinh yeu cua minh "},
{6, b, norm, inc, "Toi duoc biet den la mot nguoi co thai do lac quan, nhiet tinh, \"khong that vong\" va \"khong bao gio bo cuoc\"."},
{4, b, norm, inc, "Cuoc song toi da tung trai qua nhung thang tram, nhung luc \"len voi, xuong cho\"."},
{6, a, norm, inc, "Toi cam nhan rat ro ret rang minh dang duoc dan dat. Toi co niem tin tuyet doi vao su dan dat do va di theo no."},
{4, a, norm, inc, "Kien tao cai dep trong moi thu toi lam la dieu hoan toan khong the thieu duoc voi toi. Toi luon tim cach de tro thanh \"nguoi nghe si cua doi minh\"."},
{7, G, norm, inc, "Toi la tuyp nguoi luon \"sap dat moi viec\" va \"phan loai moi thu ngan nap\". Phuong cham cua toi la \"mot noi cho tat ca moi thu va moi thu phai o dung vi tri\" cua no."},
{1, a, norm, inc, "Cho du cuoc doi doi xu voi toi nhu nao, toi van dung vung, ngang cao dau,  kien dinh, khong so hai, vung vang va kien tri den cung."},
{1, b, norm, inc, "Toi khong ngan ngai do du pha bo nhung cai cu va khong can thiet hon da so moi nguoi."},
{2, G, norm, inc, "Toi gan nhu luc nao cung <i>dong hoa</i> (ngay lap tuc va sau sac) nhung cam xuc va tinh trang the tri cua nguoi khac. Hap thu duoc chung nhu the chung la cua chinh ban than minh."},
{1, a, norm, inc, "Toi la mot nha lanh dao bam sinh."},
{3, a, norm, inc, "Tren het, toi la mot *<i>nha tu tuong</i>*, biet su dung su thong minh cua minh de suy nghi o tam  * nhung khai niem rong nhat * ve bat ky chu de nao toi dang nghien cuu."},
{1, a, norm, inc, "So voi nhung nguoi khac, toi co mot y chi vo cung manh me va vung vang."},
{7, b, norm, inc, "Toi la mot \"nguoi to chuc\" - noi dung hon, la mot \"nguoi tai to chuc\" - mot nguoi co the manh chu yeu o viec thiet ke, xay dung hoac quan ly hieu qua tat ca cac chi tiet cua bat cu loai to chuc moi nao (du lon hay nho) duoc can den de giup nhan loai giai quyet nhieu van de - cho du la ve phap luat, tai chinh, xa hoi, ton giao hay chinh phu v.v."},
{5, G, norm, inc, "Toi dac biet gioi toan ung dung, mot nganh toan doi hoi tinh toan chinh xac va thanh thao cac phep toan so hoc khac nhau von co the ung dung vao giai quyet cac van de thuc te."},
{1, G, norm, inc, "Khi toi so sanh ban than minh voi nguoi khac, toi co the thanh that noi rang toi la mot nguoi co suc manh khac thuong - manh me va kien dinh la hai trong so cac pham chat <i>chi huy</i> cua toi."},
{4, b, norm, inc, "Toi chien dau, roi lai dan hoa, roi lai chien dau, va roi lai lam hoa. Duong nhu toi khong the nao lam gi khac ngoai hai viec do."},
{4, b, norm, inc, "Toi thich nhung gi kich tinh, va cuoc song cua toi chua day nhung trai nghiem kich tinh va nhung mau thuan ve cam xuc."},
{2, b, norm, inc, "Hay cho toi *<i>Minh triet</i>* tren het moi thu; Toi coi trong no hon so voi rat nhieu pham chat thiet yeu khac nhu quyen luc, bac ai, thong tue, tham my, kien thuc, ly tuong, trat tu, v.v ... Minh triet la tren het."},
{1, G, norm, inc, "Toi la nguoi dinh huong nhung nguoi khac."},
{4, b, norm, inc, "Hon hau het moi nguoi, cuoc doi toi day nhung xung dot (noi tam, ben ngoai hoac ca hai), toi rat hay cam thay bi giang xe giua cac luc doi lap."},
{4, b, norm, inc, "Khi toi lam mot viec gi do, cach tiep can dac trung cua toi thuong la bung len hanh dong khong ngung, sau do la khoang thoi gian tho o gan nhu tuong duong (cho du dang ra toi nen hanh dong). Toi chi la khong the bat minh lam mot dieu gi do. Va sau do, dot nhien, tam trang toi thay doi, va toi lai lao vao hanh dong."},
{5, G, norm, inc, "Toi thuong giai quyet cac van de hoac nhiem vu bang phuong phap dinh luong. Toi phan tich hoac danh gia mot van de hay mot nhiem vu tren phuong dien cac con so, va xac dinh mot giai phap phu hop."},
{2, b, norm, inc, "Tat ca cac con duong den trung tam cua ban the - cho du la con duong cua quyen luc, bac ai, thong tue, tham my, kien thuc, xuat than, huyen thuat, v.v. - deu co gia tri nhu nhau, nhung con duong cua toi la vinh vien kien tri theo duoi minh triet tu tam thuan khiet, giac ngo hoan toan va chan ly tuyet doi."},
{4, a, norm, inc, "Toi chi don gian la yeu mau sac va co kha nang cam nhan mau sac tot ma toi the hien bang cach nay hay cach khac vao moi luc."},
{6, a, norm, inc, "Khi toi can nhac tat ca cac cach khac nhau de nang do nhan loai, toi nghi long tin va niem tin vao Thuong De, vao Dang Toi cao hoac Nguon goc Chan ly nao do  la dieu quan trong nhat."},
{1, b, norm, inc, "Toi da san sang, duoc trang bi va co khuynh huong thuc day cac van de va dua ra toi hau thu bat cu khi nao toi nghi la can thiet."},
{5, a, norm, inc, "Toi de cao tinh chinh xac hoan toan, thuc su, cho du khi nhung nguoi khac co the mat kien nhan voi muc do chinh xac va dung dan do."},
{7, a, norm, inc, "Toi luon rat can than trong viec xu ly cac van de cho du la cac chi tiet nho nhat, toi muon moi thu phai hoan hao."},
{2, G, norm, inc, "Toi luon tim cach phat trien su thau hieu hoan toan ve con nguoi, su thau hieu sau sac, truc giac, tu \"trai tim den trai tim\"."},
{5, a, norm, inc, "Bat cu khi nao toi tien hanh mot nghien cuu, toi deu thuc hien mot cach nghiem tuc va co he thong, hoan toan nam vung moi chi tiet thuc te cua de tai."},
{1, Gb, norm, inc, "Toi duoc troi phu cho suc manh nang dong de vuot qua cac chuong ngai vat - va toi luon nhu vay!"},
{5, b, norm, inc, "Toi tich cuc tham gia (do nghe nghiep hoac do so thich) vao mot hoac nhieu \"nganh khoa hoc chinh xac\" nhu vat ly, hoa hoc, thien van hoc, sinh hoc, dien tu, khoa hoc may tinh, v.v. hoac nhieu chuong trinh mo rong, ket hop hoac ung dung cua cac nganh nay."},
{5, b, norm, inc, "Toi tiep can giai quyet van de thong qua quan sat truc tiep, can than kiem tra tat ca cac chi tiet. Sau do, toi bat dau thu nghiem mot cach co phuong phap, su dung quy trinh \"thu va sai\" va toi kien tri cho den khi toi tu chung minh rang toi co mot giai phap thuc su hieu qua."},
{1, b, norm, inc, "Toi la nguoi se \"chiem lay Vuong Quoc Thien Duong bang bao to\"."},
{2, G, norm, inc, "Toi co kha nang phat trien cao de nhan thuc ro rang va voi tam nhin xa thay truoc bang truc giac. Kha nang nay dac biet nhay ben khi duoc ap dung de thau hieu con nguoi - tiem nang, dong co, cac van de va moi quan he cua ho."},
{2, a, norm, inc, "Toi co mot nhu cau tham can co de muon nuoi duong va an ui moi nguoi, ngay ca khi toi khong quen biet ho."},
{7, a, norm, inc, "Toi la mot nguoi co thoi quen len lich trinh va danh sach cac viec ; do la cach toi dam bao rang toi se lam moi thu toi phai lam, vao dung thoi diem viec do nen duoc thuc hien va trong khoang thoi gian toi phai lam dieu do."},
{3, a, norm, inc, "Mac du co nhieu thu dang de theo duoi, song toi song cuoc song cua minh chu yeu nhu mot tri thuc, chu yeu tham gia vao the gioi tu tuong."},
{1, b, norm, inc, "Toi khong thay kho khan gi khi cham dut hoac loai bo moi thu. Tren thuc te, toi thuong cam thay phan khich - dac biet la khi mot thu gi do cu va loi thoi can phai duoc loai bo de mot cai gi do moi me co the thay the no."},
{5, b, norm, inc, "Toi thich may mo va tach roi moi thu (cac thiet bi, thiet bi co khi, nhieu do vat khac, v.v.) de xem cach chung duoc lap rap voi nhau, cach chung hoat dong va cach chung co the duoc sua chua."},
{6, G, norm, inc, "Toi thuong khan nguyen voi Thuong De (hoac voi chinh toi hoac voi nguoi khac) rang toi se giu gin mot so ly tuong cao ca."},
{6, a, norm, inc, "Toi khao khat song mot cuoc song voi su trung thuc va trung thanh, tha thiet phung su nhung nguoi toi yeu thuong."},
{3, G, norm, inc, "Toi noi rat nhieu - nhieu hon hau het moi nguoi. Tam tri cua toi thuong chua day nhung suy nghi thu vi, va toi can phai bieu dat chung bang loi."},
{1, G, norm, inc, "Toi co mot kha nang noi bat de khoi xuong hanh dong, kich thich nguoi khac hanh dong. Toi giang cac cu dap bua lam cho moi thu rung dong."},
{3, a, norm, inc, "Cach hanh van cua toi thuong truu tuong, phuc tap va kha kho hieu. Toi thich su trau chuot va phan nhanh, va toi mo rong tam tri minh vao nhieu linh vuc de ket hop nhieu mach suy nghi."},
{4, a, norm, inc, "Trai nghiem ve dep xuat phat tu su sap xep hai hoa, loi cuon mot cach tham my cua ca nhan va cua moi truong xa hoi xung quanh minh la dieu toi quan trong doi voi hanh phuc cua toi."},
{7, b, norm, inc, "Nhiem vu cua toi la \"mang Thien duong den Trai dat\" - khong chi chung chung nhu tat ca nhung nguoi phung su, ma thong qua * <i>bieu hien xuat sac cua cac hinh the thien duong nguyen mau</i> *, thong qua * <i>cac hinh tuong tran the hoan hao.</i>*"},
{3, G, norm, inc, "Toi thich \"tinh toan xac suat\" - dat cuoc va phong doan ve xac suat cac kich ban co the xay ra."},
{7, a, norm, inc, "Hay de cac chi tiet thuc te cho toi; Toi se to chuc chung va thuc hien chung theo cach hieu qua nhat."},
{3, G, norm, inc, "Toi suy luan voi su kheo leo khac thuong ve cac van de co tinh truu tuong, tinh ly thuyet cao trong do mot mang luoi phuc tap cac moi quan he truu tuong phai duoc hieu mot cach ro rang va chinh xac. (Vi du: toan cao cap, triet hoc han lam, ly thuyet kinh te, du bao thi truong hang hoa va chung khoan, xu the lich su, v.v.)"},
{7, a, norm, inc, "Trong hau het moi viec toi lam, hinh thanh mot thoi quen hoac mot nghi thuc ca nhan la dieu hien nhien nhat - mot chuoi mo thuc cac hanh dong lap lai thuong xuyen co hieu qua doi voi toi va toi muon lap lai dieu do."},
{4, G, norm, inc, "Toi qua de dang thoa hiep nhung khi ma dang ra toi nen kien dinh ung ho cac nguyen tac."},
{1, a, norm, inc, "Toi la nguoi cuong quyet, rat can dam khi doi mat voi moi nguy hiem."},
{3, b, norm, inc, "Khi toi xem xet tat ca cac cach khac nhau de nang cao nhan loai, toi cho rang viec kinh doanh tot, nen kinh te thong minh; thong le su dung tien te lanh manh va co hoi tai chinh rong khap la nhung thu quan trong nhat."},
{5, G, norm, inc, "Toi thich doc cac an pham ve hoc thuat hoac ky thuat, trong do giai thich can ke chi tiet nhung tien bo trong cac linh vuc nghien cuu chuyen nganh nhat dinh."},
{1, G, norm, inc, "Khi bi bat cu ai hay bat cu dieu gi can tro, *<i>y chi chinh phuc</i> * cua toi ngay lap tuc troi day."},
{5, G, norm, inc, "Toi rat say me tinh than kham pha khoa hoc, va toi cam thay thich thu va hai long khi theo duoi nghien cuu khoa hoc."},
{1, a, norm, inc, "Toi co muc dich ro rang trong moi viec minh nghi hoac lam."},
{4, G, norm, inc, "Toi khong chi say me nghe thuat sang tao ma toi con the hien va bay to cam xuc cua minh thong qua mot so hinh thuc sang tao nghe thuat."},
{5, G, norm, inc, "Toi thuong co mot thai do rat khach quan, xuat phat tu viec nghien cuu diem tinh, vo tu, tri tue cao va co tinh phan tich cac tinh huong trong cuoc song."},
{1, a, norm, inc, "Toi co du suc manh va su doc lap de dung mot minh; Toi can su ho tro va dong hanh cua nhung nguoi khac it hon nhieu so voi hau het moi nguoi."},
{2, G, norm, inc, "Boi vi toi rat vui khi thay moi nguoi truong thanh va phat trien, nen mot trong nhung niem vui lon nhat cua toi la tro thanh mot huan su."},
{6, a, norm, inc, "Toi giu nhung nguoi toi yeu that gan ben toi; Toi cam thay het long vi ho va ket noi khong the tach roi."},
{7, b, norm, inc, "Toi la mot nguoi theo chu nghia thuc hanh nghi thuc - mot nguoi su dung nhung hanh dong co to chuc, cac suy nghi co kiem soat va tu ngu nhip nhang (cung nhu ngon ngu huyen dieu cua mau sac, am nhac va bieu tuong) de keu goi cac manh luc cao hon de ho tro nhan loai."},
{2, a, norm, inc, "Bang cach nhe nhang, an can, tinh lang va yeu thuong rieng cua minh, toi lay no lam diem de nhan manh vao su tich cuc. Hau nhu toi luon co dieu gi do tot dep de suy nghi hoac noi ve moi nguoi."},
{2, a, norm, inc, "Toi thu hut moi nguoi lai voi nhau thong qua su am ap va tu tinh, va tinh yeu cua toi giup giu moi nguoi lai voi nhau."},
{1, G, norm, inc, "Vi bat cu ly do gi, toi luon co xu huong manh me toi viec gianh the chi phoi. Toi khong do du viec ap dat tham quyen."},
{5, Ga, norm, inc, "Dau oc cua toi ti mi, phan tich ky luong, va duoc chu trong sac sao vao kien thuc thuc te va cac van de cu the, dac thu."},
{5, a, norm, inc, "Toi dua ra nghien cuu ky luong, chi tiet (nhung truc giac) voi tham chi la nhung pham vi rong nhat cua cac chu de; Mot so nguoi goi cach tiep can cua toi mo pham, nhung muc dich cua toi la dat duoc su linh hoi day du, toan dien ve tat ca cac yeu to lien quan, de tro nen nam ro mot chu de (von su dung phuong phap \"cuc ky ti mi\" ua thich cua toi), den muc toi co the hoan toan hap thu duoc chu de do."},
{6, b, norm, inc, "Khi toi theo duoi mot ly tuong cao dep, toi cong hien het minh cho ly tuong do mot cach nhat tam voi cam xuc vo cung manh liet."},
{6, G, norm, inc, "Toi kien dinh voi nhung ly tuong cao ca nhat dinh ma toi tai khang dinh mot cach thuong xuyen va nhiet hanh voi chinh minh va nhan manh voi nhung nguoi khac. Nhung ly tuong nay la \"ngoi sao dan duong\" cho toi, va nho cac ly tuong, toi vach ra lo trinh cuoc doi minh."},
{7, G, norm, inc, "Toi nghi nhung dien mao rat quan trong; Toi dau tu cong suc dang ke de co ngoai hinh dep trong tat ca cac khia canh cua cuoc song va hoan canh cua toi."},
{6, b, norm, inc, "Bat cu khi nao toi \"tham gia\" thu gi do, toi deu say me  voi su nhiet tinh khong gioi han."},
{3, b, norm, inc, "Dau oc thong minh, lanh loi cua toi rat phu hop de lam viec trong linh vuc kinh te va tai chinh."},
{5, b, norm, inc, "Khi cac su co ky thuat phat sinh, toi dong vai tro la \"nguoi giai quyet rac roi\"."},
{6, a, norm, inc, "Toi cau nguyen voi niem tin hoan toan rang nhung loi cau nguyen cua toi luon duoc Thuong De hoac Dang toi cao nao do hoi dap, va toi luon san sang chap nhan bat ky hoi dap hoac cau tra loi nao ma toi nhan duoc voi thai do khiem nhuong."},
{4, a, norm, inc, "Toi thich tao ra cac ban hoa tau, cac tac pham hoac nhung su pha tron day mau sac va mang tinh tham my, bang cach lam cho nhung thu thuong khong duoc tim thay cung nhau tro thanh mot the thong nhat day nghe thuat va giau tuong tuong."},
{4, b, norm, inc, "Cam giac that vong va \"chan nan\" thuong xuyen xuat hien trong toi, nhung tam trang toi som phan chan va toi lai \"dung day\" va san sang co gang lan nua."},
{3, b, norm, inc, "Toi thich dich chuyen tien toi cho nay hay cho khac tuy theo viec toi du doan cho nao sinh loi tot nhat."},
{3, b, norm, inc, "Ban chat thich nghi mot cach thong minh va tam nhin xa trong rong cua toi ve cac xu huong thuong mai giup cho viec kinh doanh tro thanh linh vuc bieu hien tu nhien cua toi."},
{3, b, norm, inc, "So voi nhieu nguoi, toi thich tro thanh \"nguoi thong minh\" ve tien bac - dau co, dau tu va su dung tri thong minh cua minh de tao ra loi nhuan"},
{4, G, norm, inc, "Toi nghi, viet va noi bang nhung tu ngu va hinh anh day mau sac."},
{1, b, norm, inc, "Khi toi o trong mot tinh huong ca nhan hay cong viec ma khong nen tiep tuc nhu the nua, toi muon co nhung dieu chinh quyet liet, mang tinh giai phong de ngay lap tuc ket thuc nhung dieu kien cu ky."},
{3, b, norm, inc, "Toi dac biet thich nhung khia canh cua bat ky doanh nghiep kinh doanh nao ma doi hoi toi sap xep cac giao dich mot cach kheo leo va sang tao."},
{5, a, norm, inc, "Khi toi giai thich mot dieu gi do, toi chac chan rang toi cuc ky sang suot, logic va cu the (dua ra cac vi du cu the de minh hoa quan diem cua toi), de nguoi nghe co the nam bat tung chi tiet mot cach hoan toan ro rang."},
{2, G, norm, inc, "Toi lien tuc thien dinh va nghien cuu voi muc dich dat duoc su giac ngo hoan toan va hieu sau sac ve su that - mac du su hieu biet thau dao, ton thoi gian nhu vay co the khong co ung dung thuc te ngay lap tuc trong hoan canh hien tai."},
{4, G, norm, inc, "Toi yeu mau sac; nhung toi khong chi thich no - toi *<i>che tao ra no</i>* trong tat ca nhung gi toi tao ra va toi * <i>the hien khieu mau sac cua minh</i>* trong bo quan ao toi mac."},
{7, a, norm, inc, "Toi la mot trong nhung nguoi yeu cau tat ca moi thu phai duoc thuc hien mot cach kheo leo va theo trinh tu."},
{5, b, norm, inc, "Khi toi nhin vao mot phan thiet bi, suy nghi dau tien cua toi la \"no hoat dong nhu the nao?\" sau do toi tu tim hieu."},
{3, G, norm, inc, "Toi danh rat nhieu thoi gian de truyen dat rat nhieu cac y tuong va thong tin thong qua bat ky phuong tien nao san co cho toi- cho du truyen mieng hay nhieu loai phuong tien truyen thong."},
{6, G, norm, inc, "Toi dang hanh quan ve phia Anh sang."},
{6, G, norm, inc, "Toi thuong xuyen trai nghiem cam xuc dang hien manh me."},
{2, b, norm, inc, "Toi yeu men va ton trong sau sac Duc Christ va hinh mau tinh yeu thuan khiet cua Ngai, nhung ban nang cua toi xui khien toi tim kiem su giac ngo va Tu Nhan Thu thong qua viec dung trai tim va tri thien dinh de co duoc minh triet uyen tham ma Duc Phat day."},
{7, G, norm, inc, "Toi tap trung rat nhieu vao co the, vao cac quy tac va che do giu cho co the khoe manh day suc song, hoac chu y toi cac chuong trinh giup phat huy het tiem nang cua no."},
{3, G, norm, inc, "Toi rat gioi trong viec \"len ke hoach va lap kich ban\" - hoach dinh cac buoc se lam truoc rat nhieu. Toi thuc su la mot chien luoc gia xuat sac."},
{1, G, norm, SKIP, "So voi nhung nguoi khac toi la mot ca nhan manh me - suc manh va su kien dinh la hai trong so nhung pham chat noi troi cua toi."},
{4, Ga, norm, SKIP, "Toi khong chi bi thu hut boi nghe thuat sang tao, ma toi con the hien va giai phong cam xuc cua minh thong qua mot so hinh thuc sang tao nghe thuat."},
{5, b, norm, inc, "Toi cam thay o trong cac phong thi nghiem nhu o nha, va se thich lam viec trong mot so loai phong thi nghiem, noi kien thuc duoc theo duoi va xac nhan bang thuc nghiem."},
{6, G, norm, inc, "Toi mong muon cong hien het minh cho mot muc dich ly tuong hoac cao ca."},
{7, a, norm, inc, "Toi la mot nguoi thuc su hieu gia tri va tam quan trong cua viec ton trong luat phap va \"choi dung luat\"."},
{2, b, norm, inc, "Thong qua viec nghien cuu ky luong va thien dinh sau, toi quyet tam dat toi su thau hieu mot cach truc giac, hoan toan bao gom nhung chi tiet ti mi ve ve dep kieu mau cua su tong the."},
{6, a, norm, inc, "Toi muon ton tho Thuong De, va toi tran day su ton kinh doi voi cac vi thanh va hien nhan vi dai, nhung nguoi minh chung cho cac Con Duong cua Dang Tao Hoa."},
{3, b, norm, inc, "Toi la mot nha chien thuat va chien luoc - mot nguoi luon thich co mot \"ke hoach tro choi\"."},
{6, b, norm, inc, "Toi la mot 'thap tu quan' hien dai chien dau cho mot su nghiep cao ca."},
{2, a, norm, inc, "Cach cua toi la tu do chia se anh nang rang ro cua tinh yeu chan thanh voi cang nhieu nguoi cang tot."},
{1, Gb, norm, inc, "Toi noi ra su that mot cach thang than va truc tiep; no co the lam ton thuong hoac gay phien mot so nguoi, nhung su that la su that va toi khong ngai noi ra dieu von di nhu vay."},
{3, G, norm, inc, "Dau oc sang tao va tu duy linh hoat dac biet cua toi khien toi de dang sua doi va dieu chinh cac y tuong theo moi cach; Toi rat gioi trong viec giup moi nguoi nhin moi thu tu nhieu goc do moi la, tu do giup tang them nhung lua chon cho ho."},
{7, G, REVERSE, inc, "Mot so nguoi noi rang \"dau oc toi o tren may\", nhung toi khong lo lang cho ban than hay nhung nguoi khac ve nhung gi toi cho la khong quan trong. Mac du voi nhung nguoi khac, nhung thu nhu vay thuong rat quan trong, nhung voi toi chung thuong chi la nhung chuyen vat vanh khong the khien toi phien long."},
{4, a, norm, inc, "Toi luon thay minh \"o vi tri trung lap\" - giong mot cay cau trung gian va la tac nhan giup hoa giai cac y kien trai nguoc va cac manh luc doi lap."},
{5, a, norm, inc, "Toi khong chi thich nghien cuu, ma toi con kien nhan va kien tri de theo duoi tung thong tin nho nhat den nguon cua no de dam bao tinh chinh xac trong nghien cuu cua toi."},
{4, a, norm, inc, "Toi la mot \"nguoi kien tao hoa binh\" thien nghe boi vi toi hau nhu luon co su hieu biet thuc su ve \"ca hai phia\" trong cac tinh huong mau thuan va thuc su thich qua trinh cho va nhan von thuong cang thang, tim ra nhung diem dong thuan chung can thiet de giai quyet xung dot va tao ra su hai hoa."},
{2, a, norm, inc, "So voi hau het moi nguoi, toi de dang thiet lap moi quan he tu te, am ap va yeu thuong voi nhung nguoi toi gap."},
{2, G, norm, inc, "Viec tiep can bat ky muc tieu quan trong nao mot cach kien nhan va nhay ben la mot dieu tu nhien nhat voi toi. Toi hoan thanh nhieu viec thong qua su kheo leo, chu dao va hanh dong cham rai mot cach khon ngoan."},
{7, G, norm, inc, "Toi thuc su coi trong cach thuc va phong thai lam viec. Toi la mot nguoi rat gioi trong viec lam moi thu theo \"hinh thuc tot\"."},
{2, b, norm, inc, "Mac du co nhieu cach de the hien tinh yeu, song cach rieng cua toi la ket hop, pha tron tinh yeu voi tri tue hon la chi don gian bieu lo tinh yeu don thuan, vo dieu kien."},
{5, a, norm, inc, "Khi mai me phan biet thu nay voi thu khac, toi tim cach kham pha hoac tim hieu cach phan loai hoac goi ten chinh xac bat cu thu gi ma toi dang nghien cuu, xem xet, de toi co the biet chinh xac do la thu gi va khong phai thu gi."},
{3, G, norm, inc, "Toi la mot ca nhan *<i>rat</i>* tich cuc hoat dong - nhu nhieu nguoi da noi, \"tang dong\". Co rat nhieu viec phai lam va nguoi ta khong the hoan thanh chung bang cach hanh dong cham chap duoc."},
{7, a, norm, inc, "Khi toi sap xep moi thu, toi dac biet hon hau het moi nguoi ve viec sap xep chinh xac, chuan chinh hay \"dung nhu the\"!"},
{7, b, norm, inc, "Toi lien tuc tai cau truc trat tu hien co cua moi vat, cai tien va thiet ke lai cac phuong dien thuc te cua cuoc song hang ngay - cho du trong pham vi ca nhan va cong viec, hoac trong boi canh rong lon hon."},
{1, a, norm, inc, "Toi thuong co quyen luc, suc manh o trung tam cua moi vat; do la mot vi tri hoan toan tu nhien doi voi toi."},
{6, b, norm, inc, "Khi toi thuc su khao khat dat duoc muc tieu, toi \"bung chay\" va \"chay het minh\" cho den khi toi dat duoc muc tieu."},
{3, a, norm, inc, "Toi hanh dong dua tren tien de triet hoc rang chan ly co the dat duoc ma khong can thu nghiem khoa hoc, chu yeu bang cach su dung suy nghi chat che, logic, suy dien - mien la nhung tien de cua mot nguoi la dung va ly luan cua mot nguoi la co co so, hop ly."},
{4, a, norm, inc, "Vi su bat hoa luon khien toi cam thay vo cung kho chiu, nen toi ngay lap tuc hanh dong de khoi phuc lai su hai hoa mot cach nhanh chong va kheo leo nhat co the."},
{3, a, norm, inc, "Toi la mot triet gia, mot nguoi co tri tue. Trong qua trinh tim hieu ve triet hoc sau rong, tinh kho hieu va phuc tap vo tan cua the gioi tri thuc la ngoi nha that su cua toi."},
{1, G, norm, inc, "Toi biet con duong sieu viet huong moi nguoi lam theo y chi cua toi hoac Y chi cua Dang Toi cao - thuc te la toi khien ho lam nhu vay."},
{2, a, norm, inc, "Thuong thi toi cam thay tran day su cam thong va long trac an cho tat ca moi nguoi. Nhung luc dac biet nhu vay, toi yeu va tha thu tat ca mot cach vo dieu kien, bat ke ho la ai, hay ho da lam gi."},
{7, b, norm, inc, "Toi thuc su co kha nang dua nhung thu hon loan trong cuoc song con nguoi tro lai trat tu mot cach nhip nhang, bang cach hien thuc nhung y tuong co tinh chuyen hoa qua hanh dong thuc tien va chung kien chung bieu hien hoan hao."},
{7, b, norm, inc, "Toi la mot nha \"huyen thuat\" hien dai. Quen thuoc voi cac manh luc thien than va tinh linh cua tu nhien, toi ap dung mot cach can than va co ky nang cac luat thuc tien cua y chi va tu tuong de lien ket tinh than voi vat chat vi loi ich cua nhan loai."},
{1, G, norm, inc, "Khi toi xac dinh nhung gi can phai duoc thuc hien, toi nhat dinh khong de tinh cam, cam xuc hoac su quyen luyen voi moi nguoi ngan can toi thuc hien tron ven y dinh cua minh."},
{4, b, norm, inc, "Toi vua can dam vua hen nhat (va thuong cung mot luc); va cai toi the hien tuy thuoc vao tam trang cua toi, von rat thuong thay doi."},
{4, G, norm, inc, "Bat cu khi nao cang thang tich tu, toi hau nhu luon noi hoac lam dieu gi do hai huoc de giai toa su cang thang."},
{7, a, norm, inc, "Toi nhan duoc su hai long lon tu viec nghi ra hoac tham gia vao cac chuong trinh hanh dong duoc len ke hoach ky luong, co to chuc tot, trong do cac quy trinh phu hop duoc chi tiet hoa mot cach ro rang va ket qua mong muon dat duoc cung duoc xac dinh ro."},
{3, a, norm, inc, "Toi rat thich ren luyen tri nao va the hien su tai gioi ve tri thuc."},
{4, G, norm, inc, "Cach viet va cach noi thong thuong cua toi la bay to suy nghi cua minh mot tu do, ngau hung va thuong co ca chat tho."},
{4, G, norm, inc, "Toi rat gioi trong viec gay cuoi va lam cho moi cam thay giai tri bang nhung cuoc tro chuyen sinh dong, giau tri tuong tuong, nhung toi cung co the bong tro nen noi tam hoac tham chi tram lang, u dot."},
{5, b, norm, inc, "Toi khong chi quan tam sau sac toi phat minh, ma con co kha nang la mot nha phat minh trong linh vuc cong nghe."},
{2, a, norm, inc, "Toi luon the hien long trac an trong tat ca cac cuoc gap go va trai nghiem trong cuoc song cua minh."},
{6, G, norm, inc, "Bat cu khi nao nhung ly tuong ap u cua toi bi thu thach, toi nhat dinh bao ve chung mot cach nhiet tinh."},
{6, a, norm, inc, "Toi thuong khao khat duoc o noi nao ma moi thu thuan khiet va hoan hao - mot \"xa hoi khong tuong\" hay co the goi la \"thien duong\"."},
{7, b, norm, inc, "Cach song cua toi co the duoc mo ta mot cach chinh xac la \"thuc hanh nghi thuc\"; nghi le, phep thuat va bieu hien ket qua la nhung chu de chinh trong tam thuc va hoat dong cua toi."},
};

const TRAIT rgAspire[cAspire] = {
{1, "Toi khao khat tro thanh mot nha lanh dao nang dong, manh me va nhan tu - manh me, vung vang va kien dinh trong viec phung su Luat Toi Cao va bao ve Dieu Thien Hao Toi Thuong."},
{2, "Toi khao khat co duoc su thau hieu sau sac, truc giac, day yeu thuong ve moi nguoi de toi co the bao duong ho, va giup ho khai mo phat huy tiem nang cao nhat cua minh."},
{3, "Toi khao khat co mot tam tri linh hoat va gioi giang de toi co the ap dung no mot cach sang tao vao nhieu viec mang tinh thach thuc ve tri tue."},
{4, "Toi khao khat the hien kha nang nghe thuat tuyet voi trong moi khia canh cuoc song cua minh, tu do truyen cam hung cho nhung nguoi khac de sang tao hon nua va co mot tinh yeu lon hon doi voi cai dep."},
{5, "Toi khao khat duoc tham gia het minh vao viec kham pha cac chia khoa va giai phap cho nhung bi an cua Tu nhien thong qua nghien cuu va thu nghiem khoa hoc tien tien."},
{6, "Toi khao khat duoc song voi niem tin manh liet va theo duoi dam me, co su trung thanh khong lay chuyen voi he gia tri va ly tuong cao nhat cua toi - \"Vien canh ve dieu Cong Chinh\" cua toi."},
{7, "Toi khao khat mang lai mot trat tu nhip nhang dep de tu su hon loan cua cuoc song con nguoi, bang cach \"hien thuc\" nhung y tuong chuyen hoa qua hoat dong thuc tien va \"nhin thau chung\" de the hien mot cach hoan hao."},
{1, "Toi khao khat pha vo xieng xich, giai phong con nguoi khoi moi hinh thuc tu day va ap buc."},
{2, "Toi khao khat co duoc su thau hieu thong thai, khon ngoan va truc giac, sau sac va day du ve cuoc song de toi co the day va khai sang cho nhung nguoi khac."},
{3, "Toi khao khat tao ra mot hoc thuyet triet hoc that toan dien, that hop ly ve su thuc, thong qua suc manh suy nghi sau sac va ly luan can than nhat."},
{4, "Toi khao khat tro thanh mot nguoi hoa giai, mot nguoi trung gian dan xep, mot \"cau noi hop nhat\"- mot nguoi hoa giai cac xung dot cua con nguoi, va la mot cong cu hoa binh."},
{5, "Toi khao khat phat minh hoac phat trien nhung co che, cong cu hoac nhung cai tien ky thuat, de bien doi va nang cao chat luong cuoc song cua con nguoi."},
{6, "Voi duc tin hoan toan va dang hien het minh, toi khao khat phung su \"Thuong De\", hoac \"ly tuong cao ca nhat\" toi co the nhan thuc."},
{7, "Toi khao khat duoc tham gia sau vao nghe thuat va khoa hoc sang tao, chuyen hoa co cau to chuc, thiet ke lai va tai cau truc cac mo hinh tuong tac con nguoi voi nhau, de \"mot the gioi tot hon\" co the nhanh chong xuat hien hon."},
};

const TRAIT rgTrait[cTrait] = {
{7, "Qua quan tam den cac quy tac va quy dinh."},
{6, "Mat kiem soat, nhiet tinh mu quang."},
{5, "Qua nhieu hoai nghi."},
{4, "Qua nhieu \"thang\" va \"tram\"."},
{7, "Cung nhac va bieu hien khong tu nhien, guong gao."},
{6, "Cam tinh qua muc."},
{7, "Su phu thuoc vao thoi quen va le thuong."},
{5, "Khong bao quat, qua chu y den tieu tiet."},
{3, "Noi qua nhieu va suy nghi thai qua, khong mang lai ich loi ro rang."},
{2, "Thich duoc yeu men."},
{1, "Ban nga qua muc, coi trong cai toi va tu cao tu dai; luon muon tro thanh \"trung tam\"."},
{1, "Tham vong quyen luc mot cach ich ky."},
{2, "Hoc qua nhieu ma khong hanh dong."},
{3, "Ngao man ve tri tue."},
{4, "Co xu huong tu mau thuan, khong nhat quan, kho luong, hay thay doi."},
{5, "Qua chu trong phan tich va de y den tri tue."},
{6, "Qua dua dam vao nguoi khac."},
{7, "Qua quan tam den su ngan nap va chi tiet."},
{4, "Qua non nong muon thoa hiep; muon \"hoa binh voi bat cu gia nao.\""},
{6, "Qua non nong muon tin tuong va ky vong; nhiet tinh khong dung cho."},
{3, "Phi hoai nang luong vi nhung hoat dong qua muc, cuoc song dan trai va khong yen on."},
{3, "Muu mo, khong ngay thang va manh khoe."},
{2, "Hay so hai vi qua nhay cam."},
{1, "Mong muon chi phoi va kiem soat nguoi khac."},
{1, "Ngoan co, ngang nganh."},
{2, "Qua de de bo cuoc, dau hang."},
{5, "Khong chap nhan nhung y tuong khong phu hop voi nhung kien thuc co huu."},
{4, "Co gang mot cach yeu ot, bat an va hon loan."},
};

const char *rgszResponse[cResponse+1] = {"Khong co cau tra loi", "Hoan Toan Dung", "Thuong La Dung", "Dung Nhieu Hon Sai", "O Giua - Vua Dung Vua Sai", "Sai Nhieu Hon Dung", "Thuong La Sai", "Hoan Toan Sai"};
const char *rgszResponseAbbrev[cResponse+1] = {"KT", "HD", "TD", "DH", "OG", "SH", "TS", "HS"};

const char *rgszColor[cRay+3] = {"Black", "Red", "#191970", "#008000", "#c0c000", "#ffa500", "#ff3edf", "#9900CC", "White", "Yellow"};

const int rgPart[cPart] = {cInit, cStatement, cAspire2, cTrait2};


// Variables

char rgResInit[cInit][cchSzMax];
RESPONSE rgResS[cStatement];
RESPONSE rgResA[cAspire2];
RESPONSE rgResT[cTrait2];
flag fInfo = fFalse, fHTML = fFalse;
FILE *file = NULL;
int cmdarg = 0;


// Functions

long BlendColor(long kv1, long kv2, int n1, int n2)
{
  if (n2 < 1)
    n2 = 1;
  return Rgb((RgbR(kv2) - RgbR(kv1)) * n1 / n2 + RgbR(kv1),
    (RgbG(kv2) - RgbG(kv1)) * n1 / n2 + RgbG(kv1),
    (RgbB(kv2) - RgbB(kv1)) * n1 / n2 + RgbB(kv1));
}

int CchSz(const char *sz)
{
  const char *pch = sz;

  while (*pch)
    pch++;
  return (int)(pch - sz);
}

flag FEqSz(char *sz1, char *sz2)
{
  while (*sz1 && (*sz1 == *sz2))
    sz1++, sz2++;
  return *sz1 == *sz2;
}

flag FPrefixSz(char *sz1, char *sz2)
{
  while (*sz1 && (*sz1 == *sz2))
    sz1++, sz2++;
  return *sz2 == chNull;
}

void ReadLine(FILE *fileIn, char *sz)
{
  int i;
  char ch;

  for (i = 0; i < cchSzMax*2 && !feof(fileIn) && (ch = getc(fileIn)) >= ' '; i++)
    sz[i] = ch;
  sz[i] = chNull;
  if (ch != '\n')
    ch = getc(fileIn);
}

void PrintBr()
{
  if (fHTML)
    fprintf(file, "<br>");
  fprintf(file, "\n");
}

void Print(char *sz)
{
  char *pch = sz;
  flag fBold = fFalse, fItalic = fFalse;

  while (*pch) {
    if (fHTML && *pch == ' ' && (*(pch+1) == ' ' || pch == sz))
      fprintf(file, "&nbsp;");
    else if (fHTML && *pch == '\"')
      fprintf(file, "&quot;");
    else if (fHTML && *pch == '&')
      fprintf(file, "&amp;");
    else if (*pch == '~') {
      if (fHTML)
        fprintf(file, !fBold ? "<b>" : "</b>");
      fBold = !fBold;
    } else if (*pch == '`') {
      if (fHTML)
        fprintf(file, !fItalic ? "<i>" : "</i>");
      fItalic = !fItalic;
    } else
      fprintf(file, "%c", *pch);
    pch++;
  }
}

void PrintLine(char *sz)
{
  Print(sz);
  PrintBr();
}

void PrintCh(char ch, int n)
{
  if (fHTML && ch == ' ') {
    n--;
    while (n > 0) {
      fprintf(file, "&nbsp;");
      n--;
    }
    if (n >= 0)
      fprintf(file, "");
    return;
  }
  while (n > 0) {
    fprintf(file, "%c", ch);
    n--;
  }
}

void PrintLong(char *sz)
{
  char szT[cchSzMax], *pch1, *pch2, *pchT;

  if (fHTML) {
    PrintLine(sz);
    return;
  }
  for (pch1 = pch2 = sz; *pch2; pch2++) {
    if (pch2 - pch1 >= colMax-1) {
      while (pch2 > pch1 && *pch2 != ' ')
        pch2--;
      for (pchT = pch1; pchT < pch2; pchT++)
        szT[pchT - pch1] = *pchT;
      szT[pchT - pch1] = chNull;
      Print(szT);
      fprintf(file, "\n");
      pch1 = pch2 + 1;
    }
  }
  Print(pch1);
  fprintf(file, "\n");
}

void PrintLongBr(char *sz)
{
  PrintLong(sz);
  PrintBr();
}

void PrintIndent(char *sz, flag fExtra)
{
  char *pchW, *pchR, *pch1, *pch2;
  flag fLater = fFalse;

  for (pchW = pchR = sz; *pchW = *pchR; pchW++, pchR++) {
    if (*pchR == '<') {
      *pchW = '*';
      pchR += 2 + (*(pchR+1) == '/');
    }
  }
  for (pch1 = pch2 = sz; *pch2; pch2++) {
    if (pch2 - pch1 >= colMax-3 - (fExtra && fLater)*2) {
      while (pch2 > pch1 && *pch2 != ' ')
        pch2--;
      *pch2 = chNull;
      if (fExtra && fLater)
        printf(" ");
      printf("  %s\n", pch1);
      pch1 = pch2 + 1;
      fLater = fTrue;
    } 
  }
  if (fExtra && fLater)
    printf(" ");
  printf("  %s\n", pch1);
}

void PrintBanner(char *sz)
{
  if (fHTML) {
    fprintf(file, "<hr><h1>%s</h1>\n", sz);
    return;
  }
  PrintCh('*', 79);
  fprintf(file, "\n  %s\n", sz);
  PrintCh('*', 79);
  fprintf(file, "\n\n");
}

void PrintHeading(char *sz)
{
  if (fHTML) {
    fprintf(file, "<b>%s</b>", sz);
    PrintBr();
    return;
  }
  PrintLine(sz);
}

void StartColor(int i)
{
  char ch1, ch2;
  int nR, nG, nB;

  if (!fHTML)
    return;
  if (i < 0) {
    i = -i;
    ch1 = ChHex(i >> 4);
    ch2 = ChHex(i & 15);
    fprintf(file, "<font color=\"#%c%c%c%c%c%c\">", ch1, ch2, ch1, ch2, ch1, ch2);
    return;
  }
  if (i > cRay+2) {
    nR = RgbR(i); nG = RgbG(i); nB = RgbB(i);
    fprintf(file, "<font color=\"#%c%c%c%c%c%c\">", ChHex(nR >> 4), ChHex(nR & 15), ChHex(nG >> 4), ChHex(nG & 15), ChHex(nB >> 4), ChHex(nB & 15));
    return;
  }
  fprintf(file, "<font color=\"%s\">", rgszColor[i]);
}

void EndColor()
{
  if (fHTML)
    fprintf(file, "</font>");
}

void StartBkColor(int i)
{
  char ch1, ch2;
  int nR, nG, nB;

  if (!fHTML)
    return;
  if (i < 0) {
    i = -i;
    ch1 = ChHex(i >> 4);
    ch2 = ChHex(i & 15);
    fprintf(file, "<span style=\"background-color:#%c%c%c%c%c%c\">", ch1, ch2, ch1, ch2, ch1, ch2);
    return;
  }
  if (i > cRay+2) {
    nR = RgbR(i); nG = RgbG(i); nB = RgbB(i);
    fprintf(file, "<span style=\"background-color:#%c%c%c%c%c%c\">", ChHex(nR >> 4), ChHex(nR & 15), ChHex(nG >> 4), ChHex(nG & 15), ChHex(nB >> 4), ChHex(nB & 15));
    return;
  }
  fprintf(file, "<span style=\"background-color:%s\">", rgszColor[i]);
}

void EndBkColor()
{
  if (fHTML)
    fprintf(file, "</span>");
}

void StartBar(int i)
{
  if (i == 4)
    i = cRay+2;
  StartBkColor(i);
  StartColor(i);
}

void EndBar()
{
  EndColor();
  EndBkColor();
}

void StartBold()
{
  if (fHTML)
    fprintf(file, "<b>");
}

void EndBold()
{
  if (fHTML)
    fprintf(file, "</b>");
}

void StartFixed()
{
  if (fHTML)
    fprintf(file, "<font face=\"Courier\">\n");
}

void EndFixed()
{
  if (fHTML)
    fprintf(file, "</font>");
}

void StartList()
{
  if (fHTML)
    fprintf(file, "<ul>");
  else
    fprintf(file, "\n");
}

void EndList()
{
  if (fHTML)
    fprintf(file, "</ul>");
  else
    fprintf(file, "\n");
}

void PrintList(int i, char *sz)
{
  if (fHTML)
    fprintf(file, "<li>");
  StartColor(i);
  PrintLong(sz);
  EndColor();
}

int Rnd(int n)
{
  return (rand() & 16383) * n / 16384;
}

flag FFindLatest(int *part, int *ques, int *half)
{
  int i;

  *half = 0;
  for (i = 0; i < cInit; i++)
    if (rgResInit[i][0] == chNull) {
      *part = 1; *ques = i; *half = 0; return fFalse;
    }
  for (i = 0; i < cStatement; i++)
    if (rgResS[i].res == 0 || rgResS[i].resf == 0) {
      *part = 2; *ques = i; *half = 0; return fFalse;
    }
  for (i = 0; i < cAspire2; i++)
    if (rgResA[i].res == 0) {
      *part = 3; *ques = i; *half = 0; return fFalse;
    }
  for (i = 0; i < cAspire2; i++)
    if (rgResA[i].resf == 0) {
      *part = 3; *ques = i; *half = 1; return fFalse;
    }
  for (i = 0; i < cTrait2; i++)
    if (rgResT[i].res == 0) {
      *part = 4; *ques = i; *half = 0; return fFalse;
    }
  for (i = 0; i < cTrait2; i++)
    if (rgResT[i].resf == 0) {
      *part = 4; *ques = i; *half = 1; return fFalse;
    }
  *part = 1; *ques = 0;
  return fTrue;
}

void EnsureBetween(int *n, int n1, int n2)
{
  if (*n < n1) {
    printf("LOI: Luu bien tep %d duoi pham vi tu %d den %d! Gia su %d.\n", *n, n1, n2, n1);
    *n = n1;
  }
  if (*n > n2) {
    printf("LOI: Luu bien tep %d tren pham vi tu %d den %d! Gia su %d.\n", *n, n1, n2, n2);
    *n = n2;
  }
}

void Load(FILE *fileIn)
{
  char sz[cchSzMax], ch;
  int i, n;

  ReadLine(fileIn, sz);
  if (!FEqSz(sz, szDate))
    printf("CANH BAO: Luu tep ngay (%s) khong bang ngay chuong trinh (%s).\n", sz, szDate);
  for (i = 0; i < cInit; i++) {
    ReadLine(fileIn, sz);
    sprintf(rgResInit[i], "%s", sz);
  }
  for (i = 0; i < cStatement; i++) {
    ch = getc(fileIn); n = (ch - '0'); EnsureBetween(&n, 0, cResponse); rgResS[i].res = n;
    ch = getc(fileIn); n = (ch - '0'); EnsureBetween(&n, 0, cResponse); rgResS[i].resf = n;
  }
  for (i = 0; i < cAspire2; i++) {
    fscanf(fileIn, "%d", &n); EnsureBetween(&n, 0, cAspire); rgResA[i].res = n;
    fscanf(fileIn, "%d", &n); EnsureBetween(&n, 0, cAspire); rgResA[i].resf = n;
  }
  for (i = 0; i < cTrait2; i++) {
    fscanf(fileIn, "%d", &n); EnsureBetween(&n, 0, cTrait); rgResT[i].res = n;
    fscanf(fileIn, "%d", &n); EnsureBetween(&n, 0, cTrait); rgResT[i].resf = n;
  }
}

void Save(FILE *fileOut, flag fResults)
{
  FILE *fileSav = file;
  int i;

  file = fileOut;
  PrintLine(szDate);
  for (i = 0; i < cInit; i++)
    PrintLine(rgResInit[i]);
  for (i = 0; i < cStatement; i++) {
    if (fResults && i > 0 && i % ((colMax-1)/2) == 0)
      PrintBr();
    fprintf(file, "%d%d", rgResS[i].res, rgResS[i].resf);
  }
  PrintBr();
  for (i = 0; i < cAspire2; i++)
    fprintf(file, " %d %d", rgResA[i].res, rgResA[i].resf);
  PrintBr();
  for (i = 0; i < cTrait2; i++)
    fprintf(file, " %d %d", rgResT[i].res, rgResT[i].resf);
  PrintBr();
  file = fileSav;
}

flag FResponsedAlready(RESPONSE *rgres, int cres, int n, flag fFormer)
{
  int i;

  for (i = 0; i < cres; i++)
    if ((!fFormer ? rgres[i].res : rgres[i].resf) == n)
      return fTrue;
  return fFalse;
}

int GetString(char sz[cchSzMax])
{
  int cch, i, n;
  char ch = chNull;

  if (fgets(sz, cchSzMax, stdin) == NULL)
    exit(0);
  cch = CchSz(sz);
  while (cch > 0 && sz[cch-1] < ' ')
    cch--;
  sz[cch] = chNull;

  if (sz[0] == '~' && sz[1] != chNull)
    ch = sz[1];
  if (FEqSz(sz, "help")) {
    printf("\n");
    PrintLong("Chao mung den su giup do PIP III. Nhap cau tra loi cua ban cho moi cau hoi va sau do nhan <Enter>. Ban cung co the nhap bat ky lenh nao sau day:");
    printf("  undo : Neu ban tra loi sai, hay nhap \"undo\" de sao luu mot cau hoi.\n");
    printf("  back <#>: Chang han, \"back 7\" de quay lai 7 cau hoi truoc.\n");
    printf("  next <#>: Chang han, \"next 3\" de nhay den 3 cau hoi sau do.\n");
    printf("  goto <#>: Chang han, \"goto 109\" de nhay toi cau hoi #109.\n");
    printf("  part <#>: Chang han, \"part 2\" de nhay toi Phan 2: muc Nhan Dinh.\n");
    printf("  latest: Chuyen den cau hoi chua duoc tra loi dau tien hoac hien thi ket qua neu hoan thanh.\n");
    printf("  restart: Khoi dong lai bai kiem tra tu dau, xoa tat ca cac phan hoi truoc do.\n");
    printf("  help: Hien thi danh sach giup do.\n\n");
    return ecRetry;
  }
  if (FPrefixSz(sz, "undo") || FPrefixSz(sz, "back")) {
    cmdarg = -atoi(sz + 4);
    if (cmdarg >= -1) {
      cmdarg = -1;
      printf("LUU Y: Dang sao luu mot cau hoi.\n\n");
    } else
      printf("LUU Y: Dang sao luu %d cau hoi.\n\n", -cmdarg);
    return ecUndo;
  }
  if (FPrefixSz(sz, "next")) {
    cmdarg = atoi(sz + 4);
    if (cmdarg <= 1) {
      cmdarg = 1;
      printf("LUU Y: Chuyen tiep mot cau hoi.\n\n");
    } else
      printf("LUU Y: Chuyen tiep den %d cau hoi.\n\n", cmdarg);
    return ecUndo;
  }
  if (FPrefixSz(sz, "goto")) {
    cmdarg = atoi(sz + 4);
    if (cmdarg <= 1)
      cmdarg = 1;
    printf("LUU Y: Chuyen tiep den cau hoi #%d.\n\n", cmdarg);
    cmdarg--;
    return ecGoto;
  }
  if (FPrefixSz(sz, "part")) {
    cmdarg = atoi(sz + 4);
    if (cmdarg <= 1)
      cmdarg = 1;
    printf("LUU Y: Chuyen tiep den Phan %d.\n\n", cmdarg);
    return ecPart;
  }
  if (FEqSz(sz, "latest")) {
    printf("LUU Y: Chuyen den cau hoi da tra loi moi nhat.\n\n");
    return ecLate;
  }
  if (FEqSz(sz, "restart")) {
    printf("LUU Y: Bat dau lai kiem tra tu dau!\n\n");
    return ecNew;
  }
  if (ch == 'i') {
    fInfo = !fInfo;
    printf("LUU Y: Che do thong tin %s.\n", fInfo ? "ON" : "OFF");
    return ecRetry;
  }
  if (ch == 'z') {
    printf("LUU Y: Chuyen den cau hoi cuoi cung trong phan hien tai.\n\n");
    return ecLast;
  }
  if (ch == 'f') {
    printf("LUU Y: Dang hien thi ket qua.\n\n");
    return ecDone;
  }
  if (ch == 'x') {
    printf("LUU Y: Dang thoat chuong trinh.\n");
    return ecExit;
  }
  if (ch == '?' || ch == '/') {
    for (i = 0; i < cInit; i++)
      sprintf(rgResInit[i], "%s", rgInitDef[i]);
    if (ch == '?') {
      if (sz[2])
        srand(atoi(sz + 2));
      for (i = 0; i < cStatement; i++) {
        rgResS[i].res = Rnd(cResponse) + 1;
        rgResS[i].resf = Rnd(cResponse) + 1;
      }
      for (i = 0; i < cAspire2; i++) {
        do {
          rgResA[i].res = Rnd(cAspire)+1;
        } while (FResponsedAlready(rgResA, i, rgResA[i].res, fFalse));
        do {
          rgResA[i].resf = Rnd(cAspire)+1;
        } while (FResponsedAlready(rgResA, i, rgResA[i].resf, fTrue));
      }
      for (i = 0; i < cTrait2; i++) {
        do {
          rgResT[i].res = Rnd(cTrait)+1;
        } while (FResponsedAlready(rgResT, i, rgResT[i].res, fFalse));
        do {
          rgResT[i].resf = Rnd(cTrait)+1;
        } while (FResponsedAlready(rgResT, i, rgResT[i].resf, fTrue));
      }
      printf("LUU Y: Tat ca cac cau tra loi duoc ngau nhien hoa. Nhap \"latest\" de hien thi ket qua ngay bay gio.\n");
    } else {
      n = sz[2] - '0';
      if (n < 1) n = 1;
      if (n > cResponse) n = cResponse;
      for (i = 0; i < cStatement; i++) {
        rgResS[i].res = n;
        rgResS[i].resf = n;
      }
      for (i = 0; i < cAspire2; i++) {
        rgResA[i].res = cAspire-i;
        rgResA[i].resf = i+1;
      }
      for (i = 0; i < cTrait2; i++) {
        rgResT[i].res = cTrait-i;
        rgResT[i].resf = i+1;
      }
      printf("LUU Y: Tat ca cac cau tra loi duoc dat thanh lua chon #%d. Nhap \"latest\" de hien thi ket qua ngay bay gio.\n", n);
    }
    return ecRetry;
  }
  return ecNormal;
}

int GetInitResponse(int i)
{
  char sz[cchSzMax];
  int ec;

  printf("Cau hoi khoi dau #%d/%d:\n", i+1, cInit);
  if (rgResInit[i][0])
    printf("LUU Y: Ban da tra loi cau hoi nay truoc day. Ban da noi: %s\n", rgResInit[i]);
LRetry:
  printf("Nhap %s: ", rgInit[i]);
  ec = GetString(sz);
  if (ec == ecRetry)
    goto LRetry;
  else if (ec > ecNormal)
    return ec;
  if (!sz[0] && !rgResInit[i][0]) {
    printf("LOI: Vui long nhap thong tin nao do (ngay ca \"anonymous\" hoac \"unknown\").\n");
    goto LRetry;
  }
  if (sz[0])
    sprintf(rgResInit[i], "%s", sz);
  else
    printf("LUU Y: Giu cau tra loi cu cho cau hoi nay: %s\n", rgResInit[i]);
  Print("\n");
  return ec;
}

int GetStatementResponse(int s)
{
  char sz[cchSzMax];
  const STATEMENT *ps;
  int i, f, n, n1, n2, nPrev, ec;

  ps = &rgStatement[s];
  printf("Nhan dinh #%d/%d:\n", s+1, cStatement);
  sprintf(sz, "%s", ps->sz);
  PrintIndent(sz, fFalse);
  if (fInfo)
    printf("THONG TIN: Cung %d%s%s, %s, %s\n", ps->ray, (ps->nType & a) > 0 ? "a" : ((ps->nType & b) > 0 ? "b" : ""), (ps->nType & G) > 0 ? " chung" : "", ps->fReverse ? "DAO NGUOC" : "thuong", ps->fSkip ? "NHAY" : "tinh ca");
  printf("Nhan dinh tren co the la:\n");
  for (i = 1; i <= cResponse; i++)
    printf("  %d) %s: %s.\n", i, rgszResponseAbbrev[i], rgszResponse[i]);
  for (f = 0; f <= 1; f++) {
    nPrev = !f ? rgResS[s].res : rgResS[s].resf;
    if (nPrev)
      printf("LUU Y: Ban da tra loi nhan dinh nay truoc day. Ban da noi: %d (%s)\n", nPrev, rgszResponseAbbrev[nPrev]);
LRetry:
    printf("Nhap cach nhan dinh ap dung cho ban %s (1-%d): ", f == 0 ? "HIEN TAI" : "trong qua khu TRUOC KIA", cResponse);
    ec = GetString(sz);
    if (ec == ecRetry)
      goto LRetry;
    else if (ec > ecNormal)
      return ec;
    if (!sz[0] && nPrev != 0) {
      printf("LUU Y: Giu cau tra loi cu cho nhan dinh nay: #%d (%s)\n", nPrev, rgszResponseAbbrev[nPrev]);
      goto LDone1;
    }
    n = atoi(sz);
    n1 = n / 10; n2 = n % 10;
    if (f < 1 && n1 >= 1 && n1 <= cResponse && n2 >= 1 && n2 <= cResponse) {
      rgResS[s].res = n1;
      rgResS[s].resf = n2;
      printf("LUU Y: Chi dinh phan ho %d (%s) cho HIEN TAI va phan hoi %d (%s) cho TRUOC KIA.\n", n1, rgszResponseAbbrev[n1], n2, rgszResponseAbbrev[n2]);
      goto LDone2;
    }
    if (n < 1 || n > cResponse) {
      printf("LOI: Phan hoi sai. Vui long nhap mot so tu 1 den %d.\n", cResponse);
      goto LRetry;
    }
    if (!f)
      rgResS[s].res = n;
    else
      rgResS[s].resf = n;
LDone1:
    ;
  }
LDone2:
  Print("\n");
  return ec;
}

int GetAspireResponse(int t, flag fFormer)
{
  char sz[cchSzMax];
  const TRAIT *pt;
  int i, n, nPrev, ec;

  printf("NHUNG DAO TAM CAO NHAT #%d/%d:\n", t+1, cAspire-1);
  if (t > 0) {
    printf(" Nhung dao tam ban da xep hang:\n");
    for (i = 0; i < t; i++) {
      n = !fFormer ? rgResA[i].res : rgResA[i].resf;
      if (n < 1)
        continue;
      pt = &rgAspire[n-1];
      sprintf(sz, "#%d (%d): %s", i+1, n, pt->sz);
      PrintIndent(sz, fTrue);
    }
    printf("\n Nhung dao tam ban CHUA xep hang:\n");
  }
  for (i = 0; i < cAspire; i++) {
    if (FResponsedAlready(rgResA, t, i+1, fFormer))
      continue;
    pt = &rgAspire[i];
    sprintf(sz, "%d: %s", i+1, pt->sz);
    PrintIndent(sz, fTrue);
  }
  nPrev = !fFormer ? rgResA[t].res : rgResA[t].resf;
  if (nPrev)
    printf("LUU Y: Ban da tra loi dao tam nay truoc day. Ban da chon: #%d\n", nPrev);
LRetry:
  printf("Doi voi ban %s, dao tam nao o tren duoc ap dung nhieu nhat (1-%d): ", !fFormer ? "HIEN TAI" : "trong qua khu TRUOC KIA", cAspire);
  ec = GetString(sz);
  if (ec == ecRetry)
    goto LRetry;
  else if (ec > ecNormal)
    return ec;
  if (!sz[0] && nPrev != 0) {
    printf(" LUU Y: Giu nguyen cau tra loi cu cho dao tam nay: #%d\n", nPrev);
    goto LDone;
  }
  n = atoi(sz);
  if (n < 1 || n > cAspire) {
    printf("LOI: Nguyen vong sai. Vui long nhap mot so tu 1 den %d.\n", cAspire);
    goto LRetry;
  }
  if (FResponsedAlready(rgResA, t, n, fFormer)) {
    printf("LOI: Nguyen vong %d da duoc chon. Vui long nhap mot so moi.\n", n);
    goto LRetry;
  }
  if (!fFormer)
    rgResA[t].res = n;
  else
    rgResA[t].resf = n;
LDone:
  Print("\n");
  return ec;
}

int GetTraitResponse(int t, flag fFormer)
{
  char sz[cchSzMax];
  const TRAIT *pt;
  int i, n, nPrev, ec;

  printf("Cac dac tinh can chuyen hoa #%d/%d:\n", t+1, cTrait2);
  if (t > 0) {
    printf(" Cac dac tinh ban da xep hang:\n");
    for (i = 0; i < t; i++) {
      n = !fFormer ? rgResT[i].res : rgResT[i].resf;
      if (n < 1)
        continue;
      pt = &rgTrait[n-1];
      sprintf(sz, "#%d (%d): %s", i+1, n, pt->sz);
      PrintIndent(sz, fTrue);
    }
    printf("\n Cac dac tinh ban CHUA xep hang:\n");
  }
  for (i = 0; i < cTrait; i++) {
    if (FResponsedAlready(rgResT, t, i+1, fFormer))
      continue;
    pt = &rgTrait[i];
    sprintf(sz, "%d: %s", i+1, pt->sz);
    PrintIndent(sz, fTrue);
  }
  nPrev = !fFormer ? rgResT[t].res : rgResT[t].resf;
  if (nPrev)
    printf("LUU Y: Ban da tra loi dac tinh nay truoc day. Ban da chon: #%d\n", nPrev);
LRetry:
  printf("Doi voi ban %s, dac tinh nao o tren ap dung nhieu nhat (1-%d): ", !fFormer ? "HIEN TAI" : "trong qua khu TRUOC KIA", cTrait);
  ec = GetString(sz);
  if (ec == ecRetry)
    goto LRetry;
  else if (ec > ecNormal)
    return ec;
  if (!sz[0] && nPrev != 0) {
    printf("NLUU Y: Giu cau tra loi cu cho dac tinh nay: #%d\n", nPrev);
    goto LDone;
  }
  n = atoi(sz);
  if (n < 1 || n > cTrait) {
    printf("LOI: Dac tinh sai. Vui long nhap mot so tu 1 den %d.\n", cTrait);
    goto LRetry;
  }
  if (FResponsedAlready(rgResT, t, n, fFormer)) {
    printf("LOI: Dac tinh %d da duoc chon. Vui long nhap mot so moi.\n", n);
    goto LRetry;
  }
  if (!fFormer)
    rgResT[t].res = n;
  else
    rgResT[t].resf = n;
LDone:
  Print("\n");
  return ec;
}

void DoResults()
{
  POWRAY rgPower[cRay+1], rgPowerF[cRay+1], rgPowerMax[cRay+1], *pPower;
  const STATEMENT *ps;
  int rgPattern[cResponse+1][cRay+1][2], i, n, n1, n2, nMax, f, nWhat, t, nTime;
  real r;
  char sz[cchSzMax*2], ch, *pch;

  ClearB(rgPower, sizeof(rgPower));
  ClearB(rgPowerF, sizeof(rgPowerF));
  ClearB(rgPowerMax, sizeof(rgPowerMax));
  ClearB(rgPattern, sizeof(rgPattern));
  for (i = 0; i < cStatement; i++) {
    ps = &rgStatement[i];
    if (ps->fSkip)
      continue;
    for (f = 0; f <= 1; f++) {
      n = !f ? rgResS[i].res : rgResS[i].resf;
      if (n < 1)
        continue;
      if (ps->fReverse)
        n = cResponse+1 - n;
      n = cResponse - n;
      pPower = !f ? rgPower : rgPowerF;
      if (ps->nType & a) {
        pPower[ps->ray].typea += n;
        if (f) rgPowerMax[ps->ray].typea += cResponse-1;
      }
      if (ps->nType & b) {
        pPower[ps->ray].typeb += n;
        if (f) rgPowerMax[ps->ray].typeb += cResponse-1;
      }
      if (ps->nType & G) {
        pPower[ps->ray].gen += n;
        if (f) rgPowerMax[ps->ray].gen += cResponse-1;
      }
      pPower[ps->ray].tot += n;
      if (f) rgPowerMax[ps->ray].tot += cResponse-1;
    }
  }

  PrintBanner("KET QUA PIP III");
  PrintLine("PIP III: HO SO NHAN DANG CA NHAN phien ban 3.");
  PrintLine("Ban quyen (c) 2010-2021 cua Michael Robbins va Rick Good. Da dang ky Ban quyen.");
  sprintf(sz, "Phat hanh chuong trinh PC: %s", szDate);
  PrintLine(sz);
  if (!fHTML)
    PrintLong("Web: https://sevenray.org/education/");
  else
    fprintf(file, "De biet them thong tin, hay truy cap: <a href=\"https://sevenray.org/education/\">https://sevenray.org/education/</a><br>\n");
  PrintBr();

  PrintLongBr("HO SO NHAN DANG CA NHAN Phien ban III duoc thiet ke de mo ta pham nga va Sieu Nga hoac Linh Hon cua ban duoi dang mot so nang luong tinh te thiet yeu von chi phoi tat ca su song tren trai dat. Theo nhung giao huan cua hinh thuc tam ly hoc moi nhat - tam ly hoc noi mon - nhung nang luong nen tang nay phat ra tu Mat troi, va cac hanh tinh trong he mat troi cua chung ta, tu muoi hai chom sao cua hoang dao, va tu ba chom sao chinh (Sirius, Pleiades va Great Bear). Tu quan diem cua tam ly hoc noi mon, nhung nang luong ma nhan loai tien bo phai hoc ngay lap tuc de lam viec la nhung nang luong phat ra tu bay ngoi sao chinh trong Great Bear. Giao ly Minh Triet Muon Thuo khang dinh rang cac the Thien quan con lon hon nhieu so voi cac thien the khong lo, vo tri vo giac; trai lai, theo khoa hoc noi mon, cac hanh tinh, cac ngoi sao va cac chom sao duoc cho la \"cac the hong tran\" cua cac Thuc the thien the vi dai Co su thong tue va anh huong to lon doi voi tat ca con nguoi, va nhieu dang song khac do lon lon hon va nho hon.");
  PrintLongBr("Trong khi co nhung Thuc the co anh huong trong he vu tru cuc bo (cu the la nhieu hanh tinh va tieu hanh tinh chua duoc kham pha trong he mat troi cua chung ta va nhieu ngoi sao lan can deu co anh huong \"Hoa chiem tinh\"), hinh thuc hien tai cua HO SO NHAN DANG CA NHAN III chi de cap den anh huong nang luong cua cac Thuc the quan trong nhat trong so cac Thuc the nay: bay ngoi sao chinh (Thuc the) cua Great Bear, tu do phat ra bay nang luong duoc goi la Bay Cung.");

  PrintBanner("THONG TIN NHAN KHAU HOC");
  for (i = 0; i < cInit; i++) {
    sprintf(sz, "%s: %s", rgInit2[i], rgResInit[i]);
    PrintLine(sz);
  }
  PrintBr();

  for (nWhat = 0; nWhat < 3; nWhat++) {
    switch (nWhat) {
    case 0: pch = "CUNG"; break;
    case 1: pch = "KIEU MAU CUNG"; break;
    case 2: pch = "DUONG LOI CUNG"; break;
    }
    PrintBanner(pch);

  if (nWhat == 0) {
  } else if (nWhat == 1) {
    PrintLongBr("Mac du co le co \"bay\" kieu mau lien quan den moi trong so Bay Cung, PIP III chi nhan manh hai kieu mau chinh tuong quan voi nhung goi y ma Huan Su Tay Tang da dua ra trong nhieu cuon sach cua Ngai. Trong mot so truong hop, su phan biet giua LOAI A va LOAI B lien quan den muc do \"truu tuong\" hoac muc do \"cu the hoa\" nhu trong truong hop cua Loai 3A va 3B, va Loai 5A va 5B, nhung khong phai trong tat ca cac su phan biet ve loai. Doi voi cac muc dich thuc te, chung ta co the nghi ve hai loai cung duoc cung cap nhu hai loai \"thuong xuyen xuat hien\" lien quan den mot cung cu the.");
    PrintLongBr("Can hieu rang ban co the dat diem cao o mot \"loai\" cua mot cung nhat dinh va diem thap o \"loai\" khac. Vi vay, thong thuong, nhung nguoi tren cung mot cung co the khong nhan ra nhau boi vi ho la hinh mau cua cac loai khac nhau. Chang han, Chan Su DK da dua ra ba loai lien quan den cung thu hai (dac biet la linh hon cung hai). Nhung nguoi o trang thai Minh Triet hoac Bac Ai cua cung hai co the khong nhan ra nhau ngay lap tuc vi thuoc \"cung cung\".");
    PrintLongBr("Mot so nguoi su dung PIP da nhan thay So Do Loai Cung tham chi con bieu thi nhung cung cua ho (tuc la Cong thuc Cung cua ho) hon la nhung So Do Cung (coi nhung cung la bay phan chia nang luong vi dai). Mot dieu chac chan la: neu ban dat diem rat cao o mot trong cac loai cua mot cung (ngay ca khi khong thuoc loai khac) thi cung do co the manh trong he thong nang luong cua ban. Ngoai le duy nhat co the la diem so cao tren R1A, dieu nay duong nhu goi len diem so cao ngay ca tu nhung nguoi \"khong\" co mot cung R1 manh me - nhung \"long ao uoc\" ho da co.");
    PrintLong("Bieu do nay co muoi bon thang vach, bieu hien hai loai chinh cho moi cung. De co mo ta thuc su chi tiet ve cac loai, vui long nghien cuu phan \"Tam Tham cua Thuong De\" tap I va II. Cac phan chia loai cung nhu sau:");
    StartList();
    PrintList(1, "~R1A:~ Kieu mau Manh va Kien Dinh cua cung dau tien, tuong quan nhieu voi anh huong cua Vulcan va Sao Tho. Thang diem nay thuong co xu huong qua cao trong truong hop nhieu nguoi. Khi xac thuc, no chi ra kha nang tru on dinh bat chap su tan cong cua cac manh luc doi lap.");
    PrintList(1, "~R1B:~ Kieu mau Huy Diet cua cung dau tien, tuong quan nhieu voi anh huong cua Sao Diem Vuong va co le la Sao Hoa. Rat hiem khi nhung nguoi chon R1A vi ly do \"mong muon xa hoi\" (hoac 'mong muon huyen bi') lai chon R1B vi nhung ly do tuong tu. Khi R1B cao, no gan nhu chac chan chi ra su hien dien cua cung dau tien trong he thong nang luong va co le la linh hon cung mot.");
    PrintList(2, "~R2A:~ Kieu mau bac ai thuan khiet cua R2. Nhung nguoi the hien manh me Trang Thai Christ cua cung hai se ghi diem cao tren Kieu Mau Cung nay. Kieu mau nay thoat dau co the hoi thieu o nhung nguoi co cach tiep can bao gom mot cach tinh than, noi dung ra la theo duoi Minh Triet.");
    PrintList(2, "~R2B:~ Kieu Mau Minh Triet cua R2. Nhung nguoi the hien manh me Trang Thai Duc Phat cua cung hai se ghi diem cao tren Kieu Mau Cung nay. Kieu nay co the hoi thieu bieu hien cua bac ai, thay vao do nhan manh vao minh triet, mac du trong he mat troi cua chung ta, Bac Ai va Minh Triet kho co the tach roi nhau.");
    PrintList(3, "~R3A:~ Kieu Mau tu duy truu tuong cua R3: Kieu mau  nay co xu huong duoc nang cao o nhung nguoi thich suy nghi truu tuong lien quan den triet hoc, toan hoc, kinh te hoac noi chung la cac mon dau co. O nhung ca nhan nhu vay, viec tiep xuc voi nguyen tu manas thuong ton co xu huong duoc kich hoat thong qua mot khia canh cua antahkarana (Cau vong).");
    PrintList(3, "~R3B:~ Kieu mau kinh doanh va hoat dong cua R3. Kieu mau 3B duoc chu trong ve mat vat chat hon loai 3A, va duoc dac trung boi cuoc song hoat dong ben ngoai tuyet voi - ca tren coi hong tran va trong ha tri cu the.");
    PrintList(4, "~R4A:~ Kieu mau Hai Hoa va Ve Dep cua R4. Loai R4A co xu huong lam viec nhay cam va tham my huong toi su hai hoa hon la tham gia vao xung dot.");
    PrintList(4, "~R4B:~ Kieu Mau Kich Tinh, Xung Dot cua R4. Loai R4B song trong nhieu cuoc dau tranh giua cac manh luc doi lap - ca ben trong moi truong va ben trong he thong nang luong cua con nguoi. Khuynh huong cuoi cung co the huong toi viec dat duoc su hoa hop, nhung khong phai khong co xung dot kich tinh dang ke nhu la tien de cua su hoa hop can dat duoc.");
    PrintList(5, "~R5A:~  Kieu Mau Phan Tich, Phan Bien cua R5. Loai R5A co cach tiep can nhay ben, phan tich va den nhieu van de. No duoc biet den voi su thong thao chi tiet thuc te va cac mon hoc chuyen biet.");
    PrintList(5, "~R5B:~ Kieu Mau Co Khi, Sang Che cua R5. Loai R5B co xu huong ung dung thuc te tat ca nhung gi no biet - dac biet la trong linh vuc co khi, dien tu va noi chung, trong tat ca cac linh vuc ky thuat cua kien thuc ung dung.");
    PrintList(6, "~R6A:~ Kieu Mau Sung Tin cua R6. Kieu 6A the hien kieu nhe nhang hon cua cung sau, bieu hien nhieu hon nang luong Hai vuong tinh. No dai dien cho mot loai nang luong nghieng ve su dinh mac sau sac voi cac doi tuong cua su sung tin, va chu nghia ly tuong sieu viet.");
    PrintList(6, "~R6B:~ Kieu Mau Sot Sang, Nong Bong cua R6. Loai 6B the hien cac manh luc tuong quan voi hanh tinh Sao Hoa hon la Sao Hai Vuong. Ban chat du doi va thuong pha hoai khi ly tuong cua no bi thu thach. Loai nang luong cung sau nay thuong co the tham gia vao viec dan ap nhung nguoi khong co chung ly tuong, tam nhin va tin nguong va duoc trao cho su cuong tin.");
    PrintList(7, "~R7A:~  Kieu Mau R7 co To Chuc Cao. Kieu Mau 7A thuong tuong quan voi mot pham nga cung bay. Tinh yeu voi su xuat hien dung, khuon mau, thoi quen va su sap xep thi ro rang. Suc manh cua sao Tho la noi bat.");
    PrintList(7, "~R7B:~ Kieu Mau Chuyen Hoa, Cai Tien cua R7. Kieu mau 7B co nhieu kha nang tuong quan voi linh hon cung bay hon la voi pham nga cung bay. Doi moi va tai to chuc duoc coi la quan trong hon viec duy tri trat tu nghiem ngat. Suc manh cua Uranus (mot hanh tinh chu yeu la cung bay) la rat noi bat.");
    EndList();
  } else if (nWhat == 2)
    PrintLongBr("Mac du co Bay Cung, chung duoc chia thanh hai dong hoac duong nang luong chinh. Duong dau tien bat nguon tu R1 va bao gom R3, R5 va R7; bon cung nay duoc goi la cung \"duong cung\". Dong thu hai (\"cung mem\") bat nguon tu R2 va bao gom R4 va R6. Thuong co the thu thap an tuong tong the ve ban than hoac ca nhan khac chi bang cach tham khao do cao tuong doi cua hai duong cung - \"duong cung\" mau do va \"duong mem\" mau xanh lam. Duong cung lien quan nhieu hon den hinh tuong va duong mem lien quan den cuoc song ben trong hinh tuong. Su can bang chung cua hai duong loi cung co the duoc danh gia mot cach hop ly bang cach tham khao cac do thi nay. Co the thay xu huong tang hoac giam chung cua hai duong nay. Ban cung co the quyet dinh mot cach tong quat xem ban co nen lam viec de dat duoc su can bang tot hon giua chung hay khong..");

  for (nTime = 0; nTime < 5; nTime++) {
    StartBold();
    Print(" ");
    switch (nWhat) {
    case 0: Print("CUNG"); break;
    case 1: Print("KIEU MAU CUNG"); break;
    case 2: Print("DUONG LOI CUNG"); break;
    }
    Print(" ");
    switch (nTime) {
    case 0: Print("HIEN TAI"); break;
    case 1: Print("TRUOC KIA"); break;
    case 2: Print("HIEN TAI va TRUOC KIA ket hop"); break;
    case 3: Print("TRUOC KIA so sanh voi HIEN TAI"); break;
    case 4: Print("TRUOC KIA khac biet voi HIEN TAI"); break;
    }
    Print(":");
    EndBold();
    PrintBr();

if (nWhat == 0) {
  if (nTime == 0) {
    PrintLongBr("Bieu do nay la mot trong nhung bieu do quan trong nhat. Chung ta thay bay do cao cua Bay Cung vi chung duoc hieu \"chung chung\", ma khong tinh den cac Kieu mau Cung - tuc la, cac kieu phu cua bat ky cung cu the nao (va co the co, tham chi, bay kieu phu sau nay se duoc xac dinh ). Cac nhan dinh tuong quan voi bieu do nay nham kiem tra cac pham chat ma tat ca nhung nguoi the hien manh me mot cung se the hien, bat ke Kieu Mau Cung.");
    PrintLongBr("Tuy nhien, de dat duoc hieu biet ve nhung cung cua ban, Nhung Cung Hien Tai \"khong\" nen duoc nghien cuu mot cach co lap. No nen duoc nghien cuu cung voi Nhung Kieu Mau Cung de dat duoc cai nhin tong quan nhat. Ngoai ra, mot nghien cuu ve bieu do chiem tinh cua ban thi khong the thieu - mac du day la mot xem xet nang cao hon.");
    PrintLongBr("Nhung Cung Hien Tai co gang ghi dap ung cua ban doi voi nhung xu huong cung \"trong thoi diem hien tai\" - thoi diem khi ban thuc hien PIP III. Luon luon co mot thoi diem hien tai va mot cau hinh cung chinh xac hop ly cho thoi diem do. Trong khi cung linh hon co the thay doi chi mot lan (hoac co the, nhieu nhat la hai lan) trong lich su lam pham cua con nguoi hang trieu nam, nhung pham nga cua ban co the bat dau thay doi trong lan lam pham hien tai sao cho mot phan ba cuoi cung cua cuoc doi ban, ban co the chung to nhung dau hieu cua cung pham nga ma ban se co trong lan lam pham tiep theo. Dieu nay cung dung voi nhung cung cua nhung van the cua ban. Vi vay, can phai hieu rang, Cong thuc Cung cua ban dang o trong trang thai thay doi trong qua trinh lam pham hien tai cua ban. Va moi van the trong he thong nang luong cua ban cung co cac cung phu.");
    PrintLongBr("Tot nhat de nghien cuu Nhung Cung Hien Tai de dua ra cai nhin tong quan ve suc manh hien tai cua Bay Cung khi chung bieu hien \"chung chung\" trong he thong nang luong cua ban tai thoi diem nay. Tuy nhien, ban khong nen cho rang cung cao nhat nhat thiet phai la cung linh hon cua ban. No co the la cung pham nga cua ban hoac tham chi la cung the tri cua ban neu ban dang tap trung tri tue rat manh. Cung phai noi rang \"mo tuong\" doi khi anh huong den su nang len cua mot thanh cu the trong bieu do. Vi du, neu ban co mong muon sau sac la tro nen manh me va kien dinh, ban co the dap ung voi nhung nhan dinh cua Cung Mot nhu the ban manh me va kien dinh hon ban thuc su la. Mot thang do tong quat khac, Nhung Cong Thuc Cung nham giup ban phan biet giua ban \"da tung nhu the nao\" va ban \"thuc su la nhu the nao bay gio\".");
    PrintLongBr("Cac yeu to chiem tinh cung co the anh huong den su nang cao cua nhung cung. Bay Cung duoc truyen qua cac dau hieu chiem tinh va hanh tinh khac nhau va do do, mot kieu nhan manh chiem tinh cu the o thoi diem hien tai co the kich thich hoac lam giam do cao cua mot cung nhat dinh trong Nhung Cung Hien Tai mot cach doc lap voi Cong thuc Cung cua ban. Day la ly do tai sao dieu rat quan trong la phai hieu chiem tinh hoc noi mon khi co gang dat duoc danh gia chinh xac ve Bieu do Cung, Cong thuc Cung cua ban. De co mot buc tranh hoan chinh hon, bieu do chiem tinh cua ban chac chan nen duoc xem xet cung voi su chung thuc cua PIP III.");
  } else if (nTime == 1) {
    PrintLongBr("Bieu do nay nham tiet lo cach Bay Cung the hien qua he thong nang luong cua ban \"tai mot thoi diem nao do trong qua khu\". Chung toi khong tim kiem danh gia cua ban ve \"bat ky thoi diem cu the nao\" trong cuoc doi cua ban, ma la mot loai \" muc trung binh chu quan\" doi lap voi cach ban tin rang minh \" da tung \"so voi \" cach hien tai ban tin rang ban la\". Rat don gian, do cao cung tren bieu do la de cho ban biet noi ban \"da o\", so voi vi tri cua ban \"bay gio\". Do do, ban se thay nhung cung nao, theo uoc tinh cua ban, phat trien manh hon, it nhieu van nhu cu, hoac da bi giam do manh trong bieu hien cua chung. Do do, khi so sanh Nhung Cung Truoc Kia voi Nhung Cung Hien Tai, ban co the thay cac xu huong cung quan trong trong cuoc song cua minh va co le cung xac dinh duoc cac xu huong chiem tinh co ban.");
  } else if (nTime == 2) {
    PrintLongBr("Bieu do nay huu ich vi no cho thay cach cac xu huong cua qua khu (TRUOC KIA) va xu huong hien tai (HIEN TAI) cua chung ta hop nhat va phoi tron; Nhung dap ung va hanh vi hien tai cua chung ta co nhieu \"hanh trang\" tu qua khu. O giai doan tien hoa cua minh, chung ta hiem khi thoat khoi nhung khuynh huong trong qua khu. Mac du chung la cua qua khu, chung song trong hien tai. Vi vay, that thu vi khi xem ket qua cua viec ket hop cac khuynh huong trong qua khu voi cac danh gia hien tai ve thai do, phan ung va hanh vi.");
  } else if (nTime == 3) {
    PrintLongBr("Bieu do nay huu ich vi no hien thi cac do cao Cung Toan The canh nhau tu TRUOC KIA den HIEN TAI. Day la mot cach khac de nhin nhan quan trong ~ nhung xu huong cung toan the ~ trong lan lam pham cu the nay.");
  } else if (nTime == 4) {
    PrintLongBr("Bieu do nay huu ich trong viec xem (theo phan tram tang hoac giam) muc do ma cuong do cung da thay doi theo thoi gian. Cuong do tuong doi cua cac cung lien tuc thay doi trong qua trinh cua mot lan lam pham nhat dinh va tu lan nay sang lan lam pham khac. Tat ca chung ta deu co '~ tat ca cac cung ~', tuy nhien su \"nhan manh\" thay doi, do do thay doi diem nhan cua bat ky cung nao da cho tuong doi so voi cac cung khac. Chung ta muon biet neu co mot xu huong nhat dinh doi voi nhung thay doi cung ma chung ta dang trai qua hoac da trai qua. Co le cung co the thay ro xu huong cung cua lan lam pham tiep theo. Giong nhu tat ca cac de tu, chung ta co the muon co ngay cang nhieu duc tinh cua cung (mac du chung ta khong cam thay dac biet lien quan den chung), nhung mot uoc tinh thuc te duoc keu goi - khong chi dua tren suy nghi mo mong hay \"chu nghia ly tuong cung\".");
  }
}

    if (nWhat == 2 && nTime == 0)
      for (n = 1; n <= 2; n++)
        for (i = n + 2; i <= cRay; i += 2) {
          rgPower[n].tot += rgPower[i].tot;
          rgPowerF[n].tot += rgPowerF[i].tot;
          rgPowerMax[n].tot += rgPowerMax[i].tot;
        }
    for (i = 1; i <= cRay; i++) {
      if (i > 2 && nWhat == 2)
        break;
      for (f = 0; f <= 1; f++) {
        if (f && nTime != 3)
          continue;
        for (t = 0; t <= 1; t++) {
        if (t && nWhat != 1)
          continue;
        if (nWhat < 1) {
          n1 = rgPowerF[i].gen;
          n2 = rgPower[i].gen;
          nMax = rgPowerMax[i].gen;
        } else if (nWhat > 1) {
          n1 = rgPowerF[i].tot;
          n2 = rgPower[i].tot;
          nMax = rgPowerMax[i].tot;
        } else if (!t) {
          n1 = rgPowerF[i].typea;
          n2 = rgPower[i].typea;
          nMax = rgPowerMax[i].typea;
        } else {
          n1 = rgPowerF[i].typeb;
          n2 = rgPower[i].typeb;
          nMax = rgPowerMax[i].typeb;
        }
        n = nTime == 0 ? n2 :
          (nTime == 1 ? n1 :
          (nTime == 2 ? n1 + n2 :
          (nTime == 4 ? n2 - n1 :
          (f ? n2 : n1))));
        nMax *= (nTime == 2 ? 2 : 1);
        r = (nMax == 0) ? 0.0 : (real)n / (real)nMax * 100.0;
        StartColor(i);
        StartFixed();
        if (nWhat < 2)
          fprintf(file, "Cung %d%s", i, nWhat != 1 ? "" : (t ? "B" : "A"));
        else {
          if (nTime != 2)
            sprintf(sz, "%s duong loi Cung", i <= 1 ? "Cung" : "Mem ");
          else
            sprintf(sz, "Duong loi cung %s", i <= 1 ? "Cung" : "Mem ");
          Print(sz);
	}
        sprintf(sz, "%s:", nTime != 3 ? "" : (f ? " Hien Tai " : " Truoc Kia"));
        Print(sz);
        if (fInfo) {
          sprintf(sz, "%4d/%2d =", n, nMax);
          Print(sz);
        }
        sprintf(sz, "%6.2f%%: ", r);
        Print(sz);
        EndFixed();
        n = n1 = fHTML ? (int)(r * 1.0) : (int)r / 2;
        if (nTime == 4) {
          n2 = n < 0 ? (fHTML ? 50 : 25) + n : (fHTML ? 50 : 25);
          StartColor(cRay+1);
          PrintCh(' ', n2);
          EndColor();
          if (n >= 0) {
            StartBar(0);
            Print(fHTML ? "" : "|");
            EndBar();
          }
          n = NAbs(n);
        }
        ch = fHTML ? ' ' : (FOdd(i) ? 'x' : 'o');
        if (!fHTML || i != 6) {
          StartBar(i);
          PrintCh(ch, n);
          EndBar();
        } else {
          for (n2 = 0; n2 < n; n2++) {
            StartBar(BlendColor(Rgb(255, 62, 223), Rgb(0, 58, 255), n2, n-1));
            fprintf(file, "&nbsp;");
            EndBar();
          }
        }
        if (nTime == 4 && n1 < 0) {
          StartBar(0);
          Print(fHTML ? "" : "|");
          EndBar();
        }
        EndColor();
        PrintBr();
        }
      }
    }
    PrintBr();
  }
  }

  PrintBanner("NHUNG DAO TAM CAO NHAT");
  PrintLong("Mac du dao tam thuong duoc cho la bat nguon tu pham nga, nhung thuc te, chung duoc kich thich, khoi day boi suc manh tu luc cua Linh hon dua pham nga huong len. Doi khi mot dao tam cao co lien quan toi nhung pham chat ma ban ly tuong hoa nhung lai khong thuc su co trong he thong nang luong hien tai cua ban. Trong truong hop nay, cac dao tam giup bu dap. Doi khi ban co the thay rang ban khong con khao khat nhung muc tieu ma ban tung khao khat - hay it nhat la khong cung muc do nhu trong qua khu. Trong moi truong hop, xep hang dao tam cua ban se giup xac nhan Cong thuc Cung cua ban va phai tuong quan voi cac khia canh khac cua PIP III.");
  StartList();
  for (i = 0; i < cAspire; i++) {
    sprintf(sz, "Dao Tam #%d (Cung %d): %s", i+1, rgAspire[i].ray, rgAspire[i].sz);
    PrintList(rgAspire[i].ray, sz);
  }
  EndList();
  StartFixed();
  StartBold();
  PrintLine("XEP HANG    HIEN TAI           TRUOC KIA");
  EndBold();
  EndFixed();
  for (i = 0; i < cAspire; i++) {
    if (i < cAspire2) {
      n1 = rgResA[i].res;
      n2 = rgResA[i].resf;
    } else {
      for (f = 0; f <= 1; f++) {
        n = 0;
        for (t = 0; t < cAspire2; t++)
          n += (!f ? rgResA[t].res : rgResA[t].resf);
        n = cAspire * (cAspire+1) / 2 - n;
        if (!f)
          n1 = (n <= cAspire ? n : 0);
        else
          n2 = (n <= cAspire ? n : 0);
      }
    }
    if (n1 < 1 || n2 < 1)
      continue;
    StartFixed();
    fprintf(file, "#%2d: ", i+1);
    StartColor(rgAspire[n1-1].ray);
    fprintf(file, "Dao tam #%2d (Cung %d)", n1, rgAspire[n1-1].ray);
    EndColor();
    Print(", ");
    StartColor(rgAspire[n2-1].ray);
    fprintf(file, "Dao tam #%2d (Cung %d)", n2, rgAspire[n2-1].ray);
    EndColor();
    EndFixed();
    PrintBr();
  }
  PrintBr();

  PrintBanner("NHUNG DAC TINH CAN CHUYEN HOA");
  PrintLong("Trong nghien cuu huyen bi hoc, nhung dac tinh nay duoc goi la \"ao cam\" va \"ao tuong\". Chung lien quan den cac pham chat tieu cuc ma cac ca nhan the hien khi ho bi anh huong manh me boi cac cung dac biet nhung khong the kiem soat hoac dinh huong chinh xac bieu hien cua cac cung nay. Nang luong khong tot cung khong xau; tat ca phu thuoc vao dieu kien cua hinh tuong ma qua do chung hoat dong. Tat ca chung ta deu co nhung dac tinh tieu cuc can duoc chuyen hoa thanh nhung dac tinh tich cuc. Nhung bieu hien tieu cuc nay phai tuong quan voi nang luong cung duoc tim thay trong Bieu do Cung cua chung ta hien gio, hoac co le trong qua khu - vi du nhu trong mot lan tai sinh truoc. Bang cach nghien cuu \"Cac dac tinh can chuyen hoa\" noi bat nhat cua minh, ban co the thu thap cac goi y quan trong ve su hien dien cua cac cung nhat dinh trong Cong thuc Cung cua ban. Co le ban cung se thay \"Cac dac tinh can chuyen hoa\" truoc day khong con gay kho khan cho ban nua. Nhin chung, ban cung co the nhan duoc nhung goi y ve cach ban co the cai thien tinh cach cua minh.");
  StartList();
  for (i = 0; i < cTrait; i++) {
    sprintf(sz, "Dac tinh #%d (Cung %d): %s", i+1, rgTrait[i].ray, rgTrait[i].sz);
    PrintList(rgTrait[i].ray, sz);
  }
  EndList();
  StartFixed();
  StartBold();
  PrintLine("XEP HANG    HIEN TAI            TRUOC KIA");
  EndBold();
  EndFixed();
  for (i = 0; i < cTrait2; i++) {
    n1 = rgResT[i].res;
    n2 = rgResT[i].resf;
    if (n1 < 1 || n2 < 1)
      continue;
    StartFixed();
    fprintf(file, "#%d: ", i+1);
    StartColor(rgTrait[n1-1].ray);
    fprintf(file, "Dac tinh #%2d (Cung %d)", n1, rgTrait[n1-1].ray);
    EndColor();
    Print(", ");
    StartColor(rgTrait[n2-1].ray);
    fprintf(file, "Dac tinh #%2d (Cung %d)", n2, rgTrait[n2-1].ray);
    EndColor();
    EndFixed();
    PrintBr();
  }
  PrintBr();

  PrintBanner("NHUNG MO HINH DAP UNG");
  PrintLongBr("Co gia tri cao trong bang nay. Cac khuynh huong phan ung xac dinh duoc chi ra va nhung khuynh huong nay co the tuong quan voi cac cung. Vi du, nhung nguoi co Cung 5 cao trong he thong nang luong cua ho, co the dang phan bien cao va hiem khi su dung danh muc Chac chan Dung, boi vi ho luon co the tim thay ngoai le. Do cao tong the cua cac cot cung cua ho co xu huong thap hon do cao cua mot so Kieu Mau Cung khac. Voi cac kieu mau cung R6, ca hai loai Chac chan Dung va Chac chan Sai co the duoc nhan manh, boi vi ho co xu huong nhin cuoc song duoi goc do cuc doan. Do cao cung doi voi cac kieu mau cung R6 co xu huong cao hon binh thuong, co le vi ho \"khong\" phan bien. Ho cung o mot muc do dang ke \"nap truoc\", co nghia la cac danh muc HD va TD se co trong luong dang ke. Khi noi den cac kieu mau cung R1, ho co the thay minh chiem doat tat ca cac cung va do do, mot lan nua, chung ta co the tim thay do cao cung cao. Cac kieu mau cung R4 co the gap kho khan khi nhin moi thu theo cach nay hay cach khac va do do, cac cau tra loi \"Dung hon Sai\", \"Giua\" hoac \"Nhieu Sai hon Dung\" co the duoc tim thay rat nhieu. Mo hinh Dap ung Cung cung co the duoc su dung de phat hien cac kieu huong ngoai va huong noi, voi do cao cao hon cho thay kieu mau truoc day (huong ngoai) va do cao tuong doi thap hon cho thay kieu mau ve sau (huong noi). Tat ca dieu nay de noi rang Mo hinh Dap ung Cung nen duoc nghien cuu de tim ra nhung manh moi ma no co the tiet lo lien quan den nhung cung uu the. Dang tien hanh phan tich them ve loai lap bang nay.");
  PrintLongBr("Cung can phai noi rang khi mot cung co so luong lon trong danh muc Chac chan Dung, cung do co the duoc cho la noi bat trong he thong nang luong. Neu cung mot cung co nhieu cau tra loi Chac chan Dung cung co mot so cau tra loi Chac chan Sai, thi nguyen nhan thuong duoc tim thay o su khac biet giua cac Kieu Mau Cung thuong co the co ban chat rat tuong phan.");
  for (f = 0; f <= 1; f++)
    for (i = 0; i < cStatement; i++) {
      n = !f ? rgResS[i].res : rgResS[i].resf;
      rgPattern[n][rgStatement[i].ray][f]++;
      rgPattern[n][0][f]++;
    }
  for (f = 0; f <= 1; f++) {
    PrintHeading(!f ? "HIEN TAI" : "TRUOC KIA");
    StartFixed();
    PrintCh(' ', 7);
    for (n = 1; n <= cRay; n++) {
      StartColor(n);
      sprintf(sz, " R%d", n);
      Print(sz);
      EndColor();
    }
    Print(" Toan bo");
    EndFixed();
    PrintBr();
    for (i = 1; i <= cResponse; i++) {
      StartFixed();
      fprintf(file, "%d) %s:", i, rgszResponseAbbrev[i]);
      for (n = 1; n <= cRay; n++) {
        StartColor(n);
        sprintf(sz, "%3d", rgPattern[i][n][f]);
        Print(sz);
        EndColor();
      }
      n = rgPattern[i][0][f];
      sprintf(sz, "%4d", n);
      Print(sz);
      if (fInfo)
        fprintf(file, "/%d", cStatement);
      fprintf(file, "=%5.2f%%", (real)n / (real)cStatement * 100.0);
      Print(": ");
      EndFixed();
      n = n * (fHTML ? 100 : 50) / cStatement;
      StartColor(-(i-1) * (192/(cResponse-1)));
      StartBkColor(-(i-1) * (192/(cResponse-1)));
      PrintCh(fHTML ? ' ' : '+', n);
      EndBkColor();
      EndColor();
      PrintBr();
    }
    PrintBr();
  }

  PrintBanner("DAP UNG NHAN DINH");
  PrintLong("Day la mot bang tong hop giup ban co the nhanh chong tim thay cau tra loi cua minh cho nhan dinh `bat ky` tren PIP III. Ban co the de dang thay cach ban da tra loi bat ky tuyen bo nao lien quan den qua khu (TRUOC KIA) va hien tai (BAY GIO). Khi can nhac nhan dinh lien quan (tu ban sao cua PIP III), ban co the xem lai cam nhan va suy nghi cua minh ve nhan dinh cu the do khi ban tham gia PIP III va lieu ban co con nghi va cam thay nhu vay khong.");
  StartList();
  for (i = 0; i < cStatement; i++) {
    sprintf(sz, "Nhan dinh #%d (Cung %d%s): %s", i+1, rgStatement[i].ray, (rgStatement[i].nType & a) > 0 ? "a" : ((rgStatement[i].nType & b) > 0 ? "b" : ""), rgStatement[i].sz);
    PrintList(rgStatement[i].ray, sz);
  }
  EndList();
  StartFixed();
  for (i = 1; i <= cResponse; i++) {
    sprintf(sz, "%d) %s = %s.", i, rgszResponseAbbrev[i], rgszResponse[i]);
    PrintLine(sz);
  }
  EndFixed();
  PrintBr();
  for (f = 0; f <= 1; f++) {
    PrintHeading(!f ? "HIEN TAI" : "TRUOC KIA");
    StartFixed();
    for (i = 0; i < cStatement; i++) {
      n = !f ? rgResS[i].res : rgResS[i].resf;
      StartColor(rgStatement[i].ray);
      sprintf(sz, "%4d:%s", i+1, rgszResponseAbbrev[n]);
      Print(sz);
      EndColor();
      if (i >= cStatement-1 || (i+1) % 10 == 0)
        PrintBr();
    }
    EndFixed();
    PrintBr();
    for (i = 1; i <= cResponse; i++) {
      fprintf(file, "%d) %s:", i, rgszResponseAbbrev[i]);
      for (t = 0; t < cStatement; t++) {
        n = !f ? rgResS[t].res : rgResS[t].resf;
        if (n == i) {
          StartColor(rgStatement[t].ray);
          sprintf(sz, " %d", t+1);
          Print(sz);
          EndColor();
        }
      }
      PrintBr();
    }
    if (!f)
      PrintBr();
  }
  PrintBr();

  PrintBanner("KET QUA CUOI CUNG");
  sprintf(sz, "Cam on ban da lam bai kiem tra PIP III. Khi ban hoan thanh bai kiem tra, mot cua so e-mail se xuat hien chua ket qua cua ban. Vui long danh mot chut thoi gian de gui tin nhan nay cho chung toi theo dia chi %sAstara@msn.com%s. Quyen rieng tu cua ban la quan trong va nhung ket qua nay duoc giu bi mat va chi duoc su dung cho cac muc dich nghien cuu va phan tich thong ke cung nhu de huong dan cac phien ban thu nghiem trong tuong lai. Neu cua so e-mail khong xuat hien, vui long gui e-mail cho chung toi theo cach thu cong tep ket qua nay, hoac chi can sao chep va dan noi dung ben duoi.", fHTML ? "<a href=\"mailto:Astara@msn.com\">" : "", fHTML ? "</a>" : "");
  if (!fHTML)
    PrintLongBr(sz);
  else
    fprintf(file, "%s<br>\n", sz);
  if (file != stdout) {
    if (fHTML)
      fprintf(file, "<blockquote>");
    StartFixed();
    StartColor(-128);
    PrintLong("[Bat dau phan van ban de gui E-mail cho chung toi.]");
    Save(file, fTrue);
    PrintLong("[Ket thuc phan van ban de gui E-mail cho chung toi.]");
    EndColor();
    EndFixed();
    if (fHTML)
      fprintf(file, "</blockquote>");
    else
      PrintBr();
  }
  sprintf(sz, "PIP III duoc cung cap mien phi va duoc ho tro boi cac khoan dong gop tu nguyen. Neu ban muon dong gop va giup cong viec tiep tuc, vui long truy cap %shttps://www.sevenray.org/donate/%s. Xin cam on!", fHTML ? "<a href=\"https://www.sevenray.org/donate/\">" : "", fHTML ? "</a>" : "");
  if (!fHTML)
    PrintLong(sz);
  else
    fprintf(file, "%s<br>\n", sz);
}

int main(int argc, char **argv)
{
  const STATEMENT *ps;
  const TRAIT *pt;
  int iPart, i, iMax, f, ec, n;
  char sz[cchSzMax*10], szMail[cchSzMax*11], *pch, *pch2, ch;
  FILE *fileIn;

  file = stdout;
  if (argc > 1) {
    fileIn = fopen(argv[1], "r");
    if (fileIn == NULL) {

    for (i = 0; i < cInit; i++)
      printf("#%d: %s\n", i+1, rgInit2[i]);
    printf("\n");
    for (i = 0; i < cStatement; i++) {
      ps = &rgStatement[i];
      printf("#%d: Ray %d%s%s, %s, %s: %s\n", i+1, ps->ray, (ps->nType & a) > 0 ? "a" : ((ps->nType & b) > 0 ? "b" : ""), (ps->nType & G) > 0 ? " chung" : "", ps->fReverse ? "DAO NGUOC" : "thuong", ps->fSkip ? "SKIP" : "tinh ca", ps->sz);
    }
    printf("\n");
    for (i = 0; i < cAspire; i++) {
      pt = &rgAspire[i];
      printf("#%d: Cung %d: %s\n", i+1, pt->ray, pt->sz);
    }
    printf("\n");
    for (i = 0; i < cTrait; i++) {
      pt = &rgTrait[i];
      printf("#%d: Cung %d: %s\n", i+1, pt->ray, pt->sz);
    }
    return 0;

    } else {

    Load(fileIn);
    fclose(fileIn);
    goto LLoaded;

    }
  }

LNew:
  ClearB(rgResInit, sizeof(rgResInit));
  ClearB(rgResS, sizeof(rgResS));
  ClearB(rgResA, sizeof(rgResA));
  ClearB(rgResT, sizeof(rgResT));

  printf("CHAO MUNG DEN VOI PIP III: HO SO NHAN DANG CA NHAN PHIEN BAN 3.\n");
  printf("Ban quyen (c) 2010-2021 boi Michael Robbins va Rick Good. Da dang ky ban quyen.\n");
  printf("Phat hanh: %s boi Walter Pullen (%s).\n", szDate, szAddr);
  printf("Viet hoa boi MFVN. Web: https://sevenray.org/education/\n\n");

  PrintLongBr("Hay go cau tra loi cho moi Cau, sau do nhan \"ENTER\". Sau khi tra loi xong Cau cuoi cung, mot cua so se hien thi ket qua. Neu tra loi sai cau nao do, hay go \"Undo\" de quay tro lai Cau hoi do va tra loi lai. Ban cung co the go \"help\" (tro giup) de co danh sach cac cau lenh khac, ho tro ban xem lai cac cau tra loi truoc do.");

  fileIn = fopen(szSave, "r");
  if (fileIn != NULL) {
    Load(fileIn);
    fclose(fileIn);
LLoaded:
    if (!FFindLatest(&iPart, &i, &f)) {
      printf("LUU Y: Ban co mot bai kiem tra dang duoc tien hanh. Go \"restart\" neu ban muon bat dau lai.\n");
      goto LNext;
    } else
      printf("LUU Y: Ban da hoan thanh tat ca cac cau tra loi. Go \"latest\" de hien thi ket qua cuoi cung.\n");
  }

  for (iPart = 1; iPart <= 4; iPart++) {
LPart:
    if (iPart > 4) {
      printf("LOI: Khong the vuot qua Phan 4.\n");
      iPart = 4;
    }
    switch (iPart) {
    case 1: PrintBanner("Phan 1/4: NHUNG CAU HOI MO DAU");   break;
    case 2: PrintBanner("Phan 2/4: NHUNG NHAN DINH");          break;
    case 3: PrintBanner("Phan 3/4: NHUNG DAO TAM CAO NHAT"); break;
    case 4: PrintBanner("Phan 4/4: NHUNG DAC TINH CAN CHUYEN HOA"); break;
    }
    iMax = rgPart[iPart-1];
    if (iPart == 3)
      PrintLongBr("Mac du dao tam thuong duoc cho la bat nguon tu pham nga, nhung thuc te, chung duoc kich thich, khoi day boi suc manh tu luc cua Linh hon dua pham nga huong len. Doi khi mot dao tam cao co lien quan toi nhung pham chat ma ban ly tuong hoa nhung lai khong thuc su co trong he thong nang luong hien tai cua ban. Trong truong hop nay, cac dao tam giup bu dap. Doi khi ban co the thay rang ban khong con khao khat nhung muc tieu ma ban tung khao khat - hay it nhat la khong cung muc do nhu trong qua khu.");
    else if (iPart == 4)
      PrintLongBr("Trong nghien cuu huyen hoc, nhung dac tinh nay duoc goi la \"ao cam\" va \"ao tuong\". Chung lien quan den cac pham chat tieu cuc ma cac ca nhan the hien khi ho bi anh huong manh me boi cac cung dac biet nhung khong the kiem soat hoac dinh huong chinh xac bieu hien cua cac cung nay. Nang luong khong tot cung khong xau; tat ca phu thuoc vao dieu kien cua hinh tuong ma qua do chung hoat dong. Tat ca chung ta deu co nhung dac tinh tieu cuc can duoc chuyen hoa thanh nhung dac tinh tich cuc. Nhung bieu hien tieu cuc nay phai tuong quan voi nang luong cung duoc tim thay trong Bieu do Cung cua chung ta hien gio, hoac co le trong qua khu - vi du nhu trong mot lan tai sinh truoc.");

    for (f = 0; f <= (int)(iPart >= 3); f++)
    for (i = 0; i < iMax; i++) {
LNext:
      iMax = rgPart[iPart-1];
      if (i < 0) {
        printf("LOI: Khong the sao luu truoc cau hoi so #1.\n");
        i = 0;
      }
      if (i >= iMax) {
        printf("LOI: Khong the chuyen tiep sau cau hoi #%d.\n", iMax);
        i = iMax-1;
      }
      switch (iPart) {
      case 1: ec = GetInitResponse(i);      break;
      case 2: ec = GetStatementResponse(i); break;
      case 3: ec = GetAspireResponse(i, f); break;
      case 4: ec = GetTraitResponse(i, f);  break;
      }
      if (ec == ecNew) {
        _unlink(szSave);
        goto LNew;
      }
      if (ec == ecDone)
        goto LDone;
      if (ec == ecExit)
        goto LExit;
      if (ec == ecLast)
        i = iMax - 2;
      if (ec == ecUndo) {
        i += cmdarg;
        if (i == -1 && iPart > 1) {
          if (f <= 0) {
            iPart--;
            iMax = rgPart[iPart-1];
            f = (iPart >= 3);
          } else
            f = 0;
          i = iMax-1;
        }
        goto LNext;
      }
      if (ec == ecGoto) {
        i = cmdarg;
        goto LNext;
      }
      if (ec == ecPart) {
        iPart = cmdarg;
        goto LPart;
      }
      if (ec == ecLate) {
        if (FFindLatest(&iPart, &i, &f))
          goto LDone;
        goto LNext;
      }
      fileIn = fopen(szSave, "w");
      if (fileIn == NULL)
        printf("LOI: Khong the tao tep tu dong luu %s!\n", szSave);
      else {
        Save(fileIn, fFalse);
        fclose(fileIn);
      }
    }
  }
  if (!FFindLatest(&iPart, &i, &f)) {
    printf("LUU Y: Da ket thuc kiem tra nhung mot so cau hoi chua duoc tra loi.\n");
    goto LNext;
  }

LDone:
  printf("THONG BAO: Da ket thuc bai kiem tra. Da tra loi tat ca cac cau hoi!\n");

  file = fopen("pip3.txt", "w");
  if (file == NULL)
    printf(" LOI: Khong the tao tep ket qua van ban thuan tuy pip3.txt!\n");
  else {
    DoResults();
    fclose(file);
    printf("LUU Y: Da tao tep ket qua van ban thuan tuy pip3.txt\n");
  }

  file = fopen("pip3.htm", "w");
  if (file == NULL)
    printf("LOI: Khong the tao tep ket qua HTML pip3.htm!\n");
  else {
    fHTML = fTrue;
    fprintf(file, "<html><body><font face=\"Times\">\n");
    fprintf(file, "<img src=\"https://sevenray.org/wp-content/uploads/2019/08/sri-logo-nav-210h.png\">\n");
    PrintBr();
    DoResults();
    fprintf(file, "</font></body></html>\n");
    fHTML = fFalse;
    fclose(file);
    printf("LUU Y: Tep ket qua HTML da tao pip3.htm\n");
    if ((int)ShellExecute(NULL, NULL, "pip3.htm", NULL, NULL, SW_SHOW) < 32) {
      printf("LOI: Khong the khoi chay trinh duyet!\n");
      if ((int)ShellExecute(NULL, NULL, "pip3.txt", NULL, NULL, SW_SHOW) < 32)
        printf("LOI: Khong the khoi chay trinh xem van ban!\n");
      else
        printf("LUU Y: Da khoi chay trinh xem de hien thi tep ket qua van ban thuan tuy pip3.txt\n");
    } else
      printf("LUU Y: Trinh duyet khoi chay de hien thi tep ket qua HTML pip3.htm\n");
  }

  file = fopen(szSave, "w");
  if (file == NULL)
    printf("LOI: Khong the tao tep luu %s!\n", szSave);
  else {
    Save(file, fFalse);
    fclose(file);
    printf("LUU Y: Da tao tep luu %s\n", szSave);
  }
  file = stdout;

  pch = sz;
  sprintf(pch, "%s%%0A", szDate); while (*pch) pch++;
  for (i = 0; i < cInit; i++) {
    for (pch2 = rgResInit[i]; *pch2; pch2++) {
      if (*pch2 == '&') {
        sprintf(pch, "%s", "%26"); while (*pch) pch++;
      } else if (*pch2 == '%') {
        sprintf(pch, "%s", "%25"); while (*pch) pch++;
      } else if (*pch2 == '"') {
        sprintf(pch, "%s", "%22"); while (*pch) pch++;
      } else {
        *pch++ = *pch2;
      }
    }
    sprintf(pch, "%s", "%0A"); while (*pch) pch++;
  }
  for (i = 0; i < cStatement; i++) {
    sprintf(pch, "%d%d", rgResS[i].res, rgResS[i].resf); while (*pch) pch++;
  }
  sprintf(pch, "%s", "%0A"); while (*pch) pch++;
  for (i = 0; i < cAspire2; i++) {
    sprintf(pch, "%%20%d%%20%d", rgResA[i].res, rgResA[i].resf); while (*pch) pch++;
  }
  sprintf(pch, "%s", "%0A"); while (*pch) pch++;
  for (i = 0; i < cTrait2; i++) {
    sprintf(pch, "%%20%d%%20%d", rgResT[i].res, rgResT[i].resf); while (*pch) pch++;
  }
  sprintf(szMail, "mailto:%s?subject=Ket%%20qua%%20PIP3%%20(Vui%%20long%%20gui%%20cho%%20chung%%20toi%%20thu%%20nay!)&body=%s", szAddr, sz);
  if ((int)ShellExecute(NULL, NULL, szMail, NULL, NULL, SW_SHOW) < 32)
    printf("LOI: Khong the khoi chay chuong trinh E-mail!\n");
  else
    printf("THONG BAO: Da soan tin nhan E-mail. Vui long gui ket qua cua ban toi: %s\n", szAddr);

LExit:
  printf("\n");
  PrintLong("Nhan <Enter> de xem lai cau tra loi cua ban tu dau bai kiem tra. Ban cung co the dong cua so nay de thoat khoi chuong trinh.");
  GetString(sz);
  FFindLatest(&iPart, &i, &f);
  goto LNext;
  return 0;
}
