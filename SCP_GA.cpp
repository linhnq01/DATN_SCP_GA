#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define KICH_THUOC_QUAN_THE 20
#define SO_THE_HE 100
#define TY_LE_DOT_BIEN 0.1
#define SO_TAP_CON 20
#define SO_PHAN_TU 20

// Định nghĩa một tập hợp
typedef struct {
    int cac_phan_tu[SO_PHAN_TU];
} TapHop;

// Định nghĩa bài toán
typedef struct {
    TapHop cac_tap_con[SO_TAP_CON];
    int tap_goc[SO_PHAN_TU];
} BaiToanPhuTap;
void khoi_tao_du_lieu(BaiToanPhuTap *bai_toan) {
    // Khởi tạo bộ sinh số ngẫu nhiên
	srand(time(NULL)); 
    // Khởi tạo tập gốc
    for (int i = 0; i < SO_PHAN_TU; ++i) {
    	// Khởi tạo ngẫu nhiên 0 hoặc 1
        bai_toan->tap_goc[i] = rand() % 2; 
    }
    // Khởi tạo các tập con
    for (int i = 0; i < SO_TAP_CON; ++i) {
        for (int j = 0; j < SO_PHAN_TU; ++j) {
        	// Khởi tạo ngẫu nhiên 0 hoặc 1
            bai_toan->cac_tap_con[i].cac_phan_tu[j] = rand() % 2; 
        }
    }
}

void in_du_lieu(BaiToanPhuTap *bai_toan) {
    printf("Tap goc: ");
    for (int i = 0; i < SO_PHAN_TU; ++i) {
        printf("%d ", bai_toan->tap_goc[i]);
    }
    printf("\nCac tap con:\n");
    for (int i = 0; i < SO_TAP_CON; ++i) {
        printf("Tap con %d: ", i + 1);
        for (int j = 0; j < SO_PHAN_TU; ++j) {
            printf("%d ", bai_toan->cac_tap_con[i].cac_phan_tu[j]);
        }
        printf("\n");
    }
}

// Hàm đánh giá - trả về số phần tử trong tập gốc mà được bao phủ bởi một tập con
int danh_gia(BaiToanPhuTap *bai_toan, TapHop tap_hop) {
    int duoc_bao_phu[SO_PHAN_TU] = {0};
    int i, j;
    for (i = 0; i < SO_TAP_CON; ++i) {
        if (tap_hop.cac_phan_tu[i] == 1) {
            for (j = 0; j < SO_PHAN_TU; ++j) {
                duoc_bao_phu[bai_toan->cac_tap_con[i].cac_phan_tu[j]] = 1;
            }
        }
    }
    int dem = 0;
    for (i = 0; i < SO_PHAN_TU; ++i) {
        if (duoc_bao_phu[i] == 1) {
            dem++;
        }
    }
    return dem;
}

// Hàm tính phù hợp
int phu_hop(BaiToanPhuTap *bai_toan, TapHop tap_hop) {
    return danh_gia(bai_toan, tap_hop);
}

// Hàm lai ghép một điểm cắt
TapHop lai_ghep(TapHop cha1, TapHop cha2) {
    int diem_cat = rand() % SO_TAP_CON;
    TapHop con;
    int i;
    for (i = 0; i < diem_cat; ++i) {
        con.cac_phan_tu[i] = cha1.cac_phan_tu[i];
    }
    for (i = diem_cat; i < SO_TAP_CON; ++i) {
        con.cac_phan_tu[i] = cha2.cac_phan_tu[i];
    }
    return con;
}

// Hàm đột biến
void dot_bien(TapHop *tap_hop, double ty_le_dot_bien) {
    int i;
    for (i = 0; i < SO_TAP_CON; ++i) {
        if ((double)rand() / RAND_MAX < ty_le_dot_bien) {
            tap_hop->cac_phan_tu[i] = 1 - tap_hop->cac_phan_tu[i]; // Đảo bit
        }
    }
}

void in_quan_the(TapHop quan_the[], int the_he) {
    printf("The he %d:\n", the_he);
    for (int i = 0; i < KICH_THUOC_QUAN_THE; ++i) {
        printf("Ca the %d: { ", i + 1);
        for (int j = 0; j < SO_TAP_CON; ++j) {
            printf("%d ", quan_the[i].cac_phan_tu[j]);
        }
        printf("}\n");
    }
}

int main() {
    // Khởi tạo dữ liệu
    BaiToanPhuTap bai_toan;
	khoi_tao_du_lieu(&bai_toan);

    // Khởi tạo quần thể
    TapHop quan_the[KICH_THUOC_QUAN_THE];
    int i, j;
    for (i = 0; i < KICH_THUOC_QUAN_THE; ++i) {
        for (j = 0; j < SO_TAP_CON; ++j) {
            quan_the[i].cac_phan_tu[j] = rand() % 2; // Khởi tạo ngẫu nhiên
        }
    }

    // Vòng lặp thuật toán di truyền
    for (i = 0; i < SO_THE_HE; ++i) {
        // Đánh giá tất cả cá thể
        int quan_the_danh_gia[KICH_THUOC_QUAN_THE];
        for (j = 0; j < KICH_THUOC_QUAN_THE; ++j) {
            quan_the_danh_gia[j] = phu_hop(&bai_toan, quan_the[j]);
        }
        
        // In quần thể hiện tại
    	in_quan_the(quan_the, i);

        // Chọn lọc cá thể cho lai ghép
        int tong_phu_hop = 0;
        for (j = 0; j < KICH_THUOC_QUAN_THE; ++j) {
            tong_phu_hop += quan_the_danh_gia[j];
        }

        TapHop quan_the_moi[KICH_THUOC_QUAN_THE];
        for (j = 0; j < KICH_THUOC_QUAN_THE; ++j) {
            // Lựa chọn bằng cách quay xe
            int phu_hop_duoc_chon = rand() % tong_phu_hop;
            int tong_phu_hop_tam = 0;
            int k;
            for (k = 0; k < KICH_THUOC_QUAN_THE; ++k) {
                tong_phu_hop_tam += quan_the_danh_gia[k];
                if (tong_phu_hop_tam >= phu_hop_duoc_chon) {
                    quan_the_moi[j] = quan_the[k];
                    break;
                }
            }
        } 
        
        //Thực hiện lai ghép và đột biến
        for (int j = 0; j < KICH_THUOC_QUAN_THE; j += 2) {
            TapHop con1 = lai_ghep(quan_the_moi[j], quan_the_moi[j + 1]);
            TapHop con2 = lai_ghep(quan_the_moi[j + 1], quan_the_moi[j]);
            dot_bien(&con1, TY_LE_DOT_BIEN);
            dot_bien(&con2, TY_LE_DOT_BIEN);
            quan_the[j] = con1;
            quan_the[j + 1] = con2;
        }
    }
    
	// In tập gốc và các tập con
    in_du_lieu(&bai_toan);
    
    // In ra giải pháp
    printf("\nGIAI PHAP: ");
    for (i = 0; i < SO_TAP_CON; ++i) {
        printf("%d ", quan_the[0].cac_phan_tu[i]);
    }
    printf("\n");

    return 0;
}
