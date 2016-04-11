#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"

HINSTANCE hInst;

typedef struct student_s {
    char name[100];
    char surname[100];
    int year;
    double score;
} student_t;

BOOL CALLBACK Dlg2Main(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {

    case WM_CLOSE:
    {
        HWND parent = GetParent(hwndDlg);
        ShowWindow(parent, 1);
        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_COPYDATA:
        {
            PCOPYDATASTRUCT cd = (PCOPYDATASTRUCT)lParam;
            student_t * pSt = (student_t *)cd->lpData;
            printf("Win2> Received student data!\n");
            printf("Student name: %s\nStudent surname: %s\n\n",
                   pSt->name,
                   pSt->surname);

            HWND nameLbl = GetDlgItem(hwndDlg, 11);
            SetWindowText(nameLbl, pSt->name);

            HWND surnameLbl = GetDlgItem(hwndDlg, 12);
            SetWindowText(surnameLbl, pSt->surname);

            char buf[10];

            HWND yearLbl = GetDlgItem(hwndDlg, 13);
            sprintf(buf, "%i", pSt->year);
            SetWindowText(yearLbl, buf);

            HWND scoreLbl = GetDlgItem(hwndDlg, 14);
            sprintf(buf, "%f", pSt->score);
            SetWindowText(scoreLbl, buf);
        }
        return TRUE;

    case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case 10: {
                    HWND parent = GetParent(hwndDlg);
                    ShowWindow(parent, 1);
                    EndDialog(hwndDlg, 0);
                }
            }

        }
        return TRUE;
    }


    return FALSE;
}

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
    }
    return TRUE;

    case WM_CLOSE:
    {
        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
            case 5: {
                printf("Win1> Send student:\n");
                student_t * stData = (student_t *)malloc(sizeof(struct student_s));
                char buf[100];

                HWND nameLbl = GetDlgItem(hwndDlg, 1);
                GetWindowText(nameLbl, buf, sizeof(buf));
                printf("name: %s\n", buf);
                strcpy(stData->name, buf);

                HWND surnameLbl = GetDlgItem(hwndDlg, 2);
                GetWindowText(surnameLbl, buf, sizeof(buf));
                printf("surname: %s\n", buf);
                strcpy(stData->surname, buf);

                HWND yearLbl = GetDlgItem(hwndDlg, 3);
                GetWindowText(yearLbl, buf, sizeof(buf));
                stData->year = atoi(buf);
                printf("year: %i\n", stData);

                HWND scoreLbl = GetDlgItem(hwndDlg, 4);
                GetWindowText(scoreLbl, buf, sizeof(buf));
                stData->score = atof(buf);
                printf("score: %f\n", stData->score);

                COPYDATASTRUCT cd;
                cd.dwData = 0; // any
                cd.cbData = sizeof(struct student_s); // size of data
                cd.lpData = (LPVOID) stData; // pointer to data

                HWND hwnd;
                hwnd = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hwndDlg, (DLGPROC)Dlg2Main);
                SendMessage(hwnd,
                            WM_COPYDATA,
                            (WPARAM)(HWND)hwndDlg,
                            (LPARAM)(LPVOID)&cd);

                ShowWindow(hwndDlg, 0);

                break;
            }
        }
    }
    return TRUE;
    }
    return FALSE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst=hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
