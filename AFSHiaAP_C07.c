#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
#include <errno.h>
#include <sys/time.h>

static const char *aslinya = "/home/safhiram/Shift4";
char charlist[1024] = "qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";
void enkripsi(char *kata);
void dekripsi(char *kata);

static int xmp_getattr(const char *path, struct stat *stbuf)
{
	char namafilesystem[1024];
	char fol[1024];
	snprintf(namafilesystem, sizeof(namafilesystem),"%s",path);
	enkripsi(namafilesystem);
	snprintf(fol,sizeof(fol),"%s%s",aslinya,namafilesystem);
	lstat(fol, stbuf);

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
	char namafilesystem[1024];
	char fol[1024];
	snprintf(namafilesystem, sizeof(namafilesystem),"%s",path);
	enkripsi(namafilesystem);
	snprintf(fol,sizeof(fol),"%s%s",aslinya,namafilesystem);
	DIR *dp;
	struct dirent *de;
	(void) offset;
	(void) fi;

	dp = opendir(fol);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		struct stat tiga;
		FILE *yap;
		char name[1024];
		char haha[1024];
		memset(name,0,1024);
		memset(haha,0,1024);
		strcpy(name,de->d_name);
		snprintf(haha,sizeof(haha),"%s%s", fol, name);
		dekripsi(name);
		
		stat(haha,&tiga);
		struct passwd *pass = getpwuid(tiga.st_uid);
    	struct group *grouptiga = getgrgid(tiga.st_gid);
		
		if((strcmp(pass->pw_name,"chipset") == 0 || strcmp(pass->pw_name,"ic_controller") == 0) && strcmp(grouptiga->gr_name,"rusak")==0)
		{
			if((tiga.st_mode & S_IRUSR) == 0 || (tiga.st_mode & S_IRGRP) == 0 || (tiga.st_mode & S_IROTH) == 0)
			{
				char isi[1024];
				char isifile[1024];
				char simpan[1024]="/filemiris.txt";
				char asli[1024]="/home/safhiram/Shift4";
				time_t waktu =time(NULL);
				enkripsi(simpan);
				strcat(asli,simpan);
				yap=fopen(asli,"a+");
				strftime(isi, 20 ,"%Y-%m-%d %H:%M:%S", localtime(&waktu));
				snprintf(isifile,sizeof(isifile), "%s%s-%d-%d-%s",path,name,pass->pw_uid,grouptiga->gr_gid,isi);
				fprintf(yap,"%s",isifile);
				remove(haha);
				fclose(yap);
				chown(asli,1000,1000);
			}
		}
		else
		{
			memset(&st, 0, sizeof(st));
			st.st_ino = de->d_ino;
			st.st_mode = de->d_type << 12;
			if (filler(buf, name, &st, 0))
				break;			
		}
	}
	closedir(dp);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
	int res,fd;
	char namafilesystem[1024];
	char fol[1024];
	snprintf(namafilesystem, sizeof(namafilesystem),"%s",path);
	enkripsi(namafilesystem);
	snprintf(fol,sizeof(fol),"%s%s",aslinya,namafilesystem);

	(void) fi;
	fd=open(fol, O_RDONLY);
	res=pread(fd, buf, size, offset);
	close(fd);
	return res;
}

static int xmp_unlink(const char *path)
{
	struct stat st = {0};
	unlink(path);
	char rb[1024]="/RecycleBin";
	enkripsi(rb);
	char folderpath[1024];
	strcpy(folderpath,aslinya);
	strcat(folderpath,rb);
	
	if (stat(folderpath, &st) == -1) {
        mkdir(folderpath,0755);
    }
	
	char ekstensi[1024]=".ekstensi";
	char timestamp[1024];
	char namafile[1024];
	char namafilebackup[1024];
	time_t waktu =time(NULL);
	strftime(timestamp, 20 ,"%Y-%m-%d_%H:%M:%S", localtime(&waktu));
	strcpy(namafilebackup,namafile);
	strcat(namafilebackup,timestamp);
	strcat(namafilebackup,ekstensi);
	enkripsi(namafilebackup);
	strcat(folderpath,namafilebackup);
	
	if(!fork())
	{
		
	}
	else
	{
		char *zip[]={"zip -m ",folderpath,NULL};
		execv("/bin/zip",zip);
	}

	return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
//	int res,fd;
//	struct stat st = {0};
//	char namafilesystem[1024];
//	char fol[1024];
//	snprintf(namafilesystem, sizeof(namafilesystem),"%s",path);
//	snprintf(fol,sizeof(fol),"%s%s",aslinya,namafilesystem);
//	fd=open(fol, O_WRONLY);
//	
//	(void) fi;
//	res=pwrite(fd, buf, size, offset);
//	
//		char folderpath[1024];
//		char backup[1024]="/Backup";
//		enkripsi(backup);
//		char ekstensi[1024]=".ekstensi";
//		char namafileawal[1024];
//		snprintf(namafileawal,sizeof(namafileawal),"%s%s",folderpath,path);
//		char namafile[1024];
//		snprintf(namafile,sizeof(namafile),"/%s_",path);
//		char namafilebackup[1024];
//		char timestamp[1024];
//		strcpy(folderpath,aslinya);
//		strcat(folderpath,backup);
//		
//		if (stat(folderpath, &st) == -1) {
//        mkdir(folderpath,0755);
//    }
//    
//		time_t waktu =time(NULL);
//		strftime(timestamp, 20 ,"%Y-%m-%d_%H:%M:%S", localtime(&waktu));
//		strcpy(namafilebackup,namafile);
//		strcat(namafilebackup,timestamp);
//		strcat(namafilebackup,ekstensi);
//		enkripsi(namafilebackup);
//		strcat(folderpath,namafilebackup);
//		rename(namafileawal,folderpath);
//		
//	close(fd);
//	return res;

	int res,fd;
	char namafilesystem[1024];
	char fol[1024];
	snprintf(namafilesystem, sizeof(namafilesystem),"%s",path);
	enkripsi(namafilesystem);
	snprintf(fol,sizeof(fol),"%s%s",aslinya,namafilesystem);

	(void) fi;
	fd=open(fol, O_WRONLY);
	res=pwrite(fd, buf, size, offset);
	close(fd);
	return res;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
	int res;
	char namafilesystem[1024];
	char fol[1024];
	snprintf(namafilesystem, sizeof(namafilesystem),"%s",path);
	enkripsi(namafilesystem);
	snprintf(fol,sizeof(fol),"%s%s",aslinya,namafilesystem);

	res=open(fol, fi->flags);
	
	close(res);
	return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
	char namafilesystem[1024];
	char fol[1024];
	snprintf(namafilesystem, sizeof(namafilesystem),"%s",path);
	enkripsi(namafilesystem);
	snprintf(fol,sizeof(fol),"%s%s",aslinya,namafilesystem);
	int res;
	if (S_ISREG(mode)) {
		res = open(fol, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode))
		res = mkfifo(fol, mode);
	else
		res = mknod(fol, mode, rdev);
		
	return 0;
}

static int xmp_chown(const char *path, uid_t uid, gid_t gid)
{
	char namafilesystem[1024];
	char fol[1024];
	snprintf(namafilesystem, sizeof(namafilesystem),"%s",path);
	enkripsi(namafilesystem);
	snprintf(fol,sizeof(fol),"%s%s",aslinya,namafilesystem);
	lchown(fol, uid, gid);
	return 0;
}

static int xmp_utimeandstart(const char *path, const struct timespec ts[2])
{
	struct timeval timevalue[2];
	char namafilesystem[1024];
	char fol[1024];
	snprintf(namafilesystem, sizeof(namafilesystem),"%s",path);
	enkripsi(namafilesystem);
	snprintf(fol,sizeof(fol),"%s%s",aslinya,namafilesystem);

	timevalue[0].tv_sec = ts[0].tv_sec;
	timevalue[0].tv_usec = ts[0].tv_nsec / 1000;
	timevalue[1].tv_sec = ts[1].tv_sec;
	timevalue[1].tv_usec = ts[1].tv_nsec / 1000;

	utimes(fol, timevalue);

	return 0;
}

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
	.write		= xmp_write,
	.open		= xmp_open,
	.mknod		= xmp_mknod,
	.chown		= xmp_chown,
	.utimens	= xmp_utimeandstart,
	.unlink 	= xmp_unlink,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}

void enkripsi(char *kata)
{
	if(!strcmp(kata,".")) return;
	if(!strcmp(kata,"..")) return;
	for(int i=0;i<strlen(kata);i++)
	{
		int k=94;
		int j=0;
		while(k>0)
		{
			if(charlist[j]==kata[i])
			{
				kata[i]=charlist[(j+17)%94];
				break;
			}
			j++; k--;
		}
	}
}

void dekripsi(char *kata)
{
	if(!strcmp(kata,".")) return;
	if(!strcmp(kata,"..")) return;
	for(int i=0;i<strlen(kata);i++)
	{
		int k=94;
		int j=0;
		while(k>0)
		{
			if(charlist[j]==kata[i])
			{
				kata[i]=charlist[(j+77)%94];
				break;
			}
			j++; k--;
		}
	}
}

