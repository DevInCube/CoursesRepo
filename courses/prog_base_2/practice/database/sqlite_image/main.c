#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sqlite3.h"

// a table for images
/*
CREATE TABLE `image` (
	`Id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`Name`	TEXT NOT NULL,
	`Data`	BLOB NOT NULL
)
*/

char * load_image(const char * imageFile, int * imageSize);
int save_image(const char * name, char * bytes, int bytesLen);

int db_save_image(sqlite3 * db, const char * name, char * bytes, int bytesLength);
const char * db_load_image(sqlite3 * db, const char * name, int * bytesLength);

int main(void) {
    sqlite3 * db = NULL;
    int rc = 0;
    const char * dbFile = "image_db.db";
    rc = sqlite3_open(dbFile, &db);  // open connection to DB
    if (SQLITE_OK != rc) {
        printf("Can't open file `%s`\n", dbFile);
        return 1;
    }

    const char * imageFile = "image.png";
    int imageSize = 0;
    char * imageBytes = NULL;

    int choice = 0;
    do {
        printf("Choose action:\n1. Save image to db.\n2. Load image from db\n");
        scanf("%i", &choice);
    } while (choice < 1 || choice > 2);

    if (1 == choice) {
        imageBytes = load_image(imageFile, &imageSize);
        if (NULL == imageBytes) {
            printf("Can't load image `%s`\n", imageFile);
            return 1;
        }

        if (0 != db_save_image(db, imageFile, imageBytes, imageSize)) {
            printf("Error inserting image to db\n");
            return 1;
        }
    } else if (2 == choice) {
        imageBytes = db_load_image(db, imageFile, &imageSize);
        if (NULL == imageBytes) {
            printf("Can't load image `%s` from db\n", imageFile);
            return 1;
        }

        const char * newImageFile = "bin/Debug/new-image.png";
        if (0 != save_image(newImageFile, imageBytes, imageSize)) {
            printf("Can't save image `%s`\n", newImageFile);
            return 1;
        }
    }

    free(imageBytes);
    sqlite3_close(db);  // close connection to DB
    return 0;
}

char * load_image(const char * imageFile, int * imageSize) {
    FILE * fileptr;
    char * buffer;
    long filelen;

    fileptr = fopen(imageFile, "rb");  // Open the file in binary mode
    if (NULL == fileptr) {
        return NULL;
    }
    fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
    filelen = ftell(fileptr);             // Get the current byte offset in the file
    rewind(fileptr);                      // Jump back to the beginning of the file

    buffer = (char *)malloc((filelen + 1) * sizeof(char)); // Enough memory for file + \0
    fread(buffer, filelen, 1, fileptr); // Read in the entire file
    fclose(fileptr); // Close the file
    *imageSize = filelen;
    return buffer;
}

int db_save_image(sqlite3 * db, const char * name, char * bytes, int bytesLength) {
    const char * sqlScript = "INSERT INTO image (name, data) VALUES (?, ?)";
    sqlite3_stmt * stmt = NULL;
    int rc = sqlite3_prepare_v2(db, sqlScript, strlen(sqlScript) + 1, &stmt, NULL);
    if (SQLITE_OK != rc) {
        printf("Can't prepare statement\n", name);
        return 1;
    }
    sqlite3_bind_text(stmt, 1, name, strlen(name), NULL);
    sqlite3_bind_blob(stmt, 2, bytes, bytesLength, NULL);

    rc = sqlite3_step(stmt);
    if (SQLITE_ERROR == rc) {
        printf("Can't execute statement. %i\n", rc);
        return 1;
    }
    sqlite3_finalize(stmt);
    return 0;
}

const char * db_load_image(sqlite3 * db, const char * name, int * bytesLength) {
    const char * sqlScript = "SELECT * FROM image WHERE name = ?";
    sqlite3_stmt * stmt = NULL;
    int rc = sqlite3_prepare_v2(db, sqlScript, strlen(sqlScript) + 1, &stmt, NULL);
    if (SQLITE_OK != rc) {
        printf("Can't prepare statement\n", name);
        return NULL;
    }
    sqlite3_bind_text(stmt, 1, name, strlen(name), NULL);

    const char * imgBytes = NULL;
    rc = sqlite3_step(stmt);
    if (SQLITE_ERROR == rc) {
        printf("Can't execute statement. %i\n", rc);
        return NULL;
    } else if (SQLITE_ROW == rc) {
        const void * data = sqlite3_column_blob(stmt, 2);
        const int dataLen = sqlite3_column_bytes(stmt, 2);
        imgBytes = malloc(dataLen);
        memcpy(imgBytes, data, dataLen);
        *bytesLength = dataLen;
    }
    sqlite3_finalize(stmt);
    return imgBytes;
}

int save_image(const char * name, char * bytes, int bytesLen) {
    FILE * write_ptr;
    write_ptr = fopen(name, "wb");
    if (NULL == write_ptr) {
        return 1;
    }
    fwrite(bytes, bytesLen, 1, write_ptr);
    fclose(write_ptr);
    return 0;
}
