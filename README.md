# SoalShift_modul4_C07
<h2>NOMOR 1</h2>
<p>Semua nama file dan folder harus terenkripsi
Enkripsi yang Atta inginkan sangat sederhana, yaitu Caesar cipher. Namun, Kusuma mengatakan enkripsi tersebut sangat mudah dipecahkan. Dia menyarankan untuk character list diekspansi,tidak hanya alfabet, dan diacak. Berikut character list yang dipakai:</p>

<p>qE1~ YMUR2"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\8s;g<{3.u*W-0</p>

<p>Misalkan ada file bernama “halo” di dalam folder “INI_FOLDER”, dan key yang dipakai adalah 17, maka:
“INI_FOLDER/halo” saat belum di-mount maka akan bernama “n,nsbZ]wio/QBE#”, saat telah di-mount maka akan otomatis terdekripsi kembali menjadi “INI_FOLDER/halo” .
Perhatian: Karakter ‘/’ adalah karakter ilegal dalam penamaan file atau folder dalam *NIX, maka dari itu dapat diabaikan</p>

<h2>JAWABAN</h2>
<p>1. Untuk membuat file system sesuai permintaan nomor satu. Terdapat beberapa fungsi yang ditunjuk, yaitu:</p>

```
static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
	.write		= xmp_write,
	.open		= xmp_open,
	.mknod		= xmp_mknod,
	.chown		= xmp_chown,
	.utimens	= xmp_utimeandstart,
};
```

<p>2. Buat fungsi enkripsi. Jika nama file berupa '.' dan '..' maka return. Jika kata tersebut mengandung kata-kata di charlist maka enkrip nama file tersebut.</p>

```
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
```

<p>3. Buat fungsi dekripsi.  Jika nama file berupa '.' dan '..' maka return. Jika kata tersebut mengandung kata-kata di charlist maka dekrip nama file tersebut.</p>

```
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
```

<p>4. Untuk membuat nama file tersebut terenkripsi sebelum di mount. lakukan enkrip pada setiap awal fungsi tersebut.</p>

```
	snprintf(namafilesystem, sizeof(namafilesystem),"%s",path);
	enkripsi(namafilesystem);
```

<p>5. Buat sebuah variabel untuk menampung isi setiap filename yang ada di folder sebelum di mount.</p>

```
	snprintf(fol,sizeof(fol),"%s%s",aslinya,namafilesystem);
```

<h2>NOMOR 3</h2>
<p>Sebelum diterapkannya file system ini, Atta pernah diserang oleh hacker LAPTOP_RUSAK yang menanamkan user bernama “chipset” dan “ic_controller” serta group “rusak” yang tidak bisa dihapus. Karena paranoid, Atta menerapkan aturan pada file system ini untuk menghapus “file bahaya” yang memiliki spesifikasi:
Owner Name 	: ‘chipset’ atau ‘ic_controller’
Group Name	: ‘rusak’
Tidak dapat dibaca
Jika ditemukan file dengan spesifikasi tersebut ketika membuka direktori, Atta akan menyimpan nama file, group ID, owner ID, dan waktu terakhir diakses dalam file “filemiris.txt” (format waktu bebas, namun harus memiliki jam menit detik dan tanggal) lalu menghapus “file bahaya” tersebut untuk mencegah serangan lanjutan dari LAPTOP_RUSAK.</p>
<h2>JAWABAN</h2>

<p>Ambil nilai owner name dan group name. JIka owner name berupa "chipset" atau "ic_controller" dan group name berupa "rusak". maka buat filemiris.txt.</p>
<p>Di dalam file miris tersebut berisi nama file, group ID, owner IS dan waktu terakhir mengakses file.</p>

```
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
```
<h2>NOMOR 5</h2>
<p> Ketika mengedit suatu file dan melakukan save, maka akan terbuat folder baru bernama Backup kemudian hasil dari save tersebut akan disimpan pada backup dengan nama namafile_[timestamp].ekstensi. Dan ketika file asli dihapus, maka akan dibuat folder bernama RecycleBin, kemudian file yang dihapus beserta semua backup dari file yang dihapus tersebut (jika ada) di zip dengan nama namafile_deleted_[timestamp].zip dan ditaruh ke dalam folder RecycleBin (file asli dan backup terhapus). Dengan format [timestamp] adalah yyyy-MM-dd_HH:mm:ss </p>

<h2>JAWABAN</h2>
Untuk folder Backup:

```
static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	int res,fd;
	struct stat st = {0};
	char namafilesystem[1024];
	char fol[1024];
	snprintf(namafilesystem, sizeof(namafilesystem),"%s",path);
	snprintf(fol,sizeof(fol),"%s%s",aslinya,namafilesystem);
	fd=open(fol, O_WRONLY);
	
	(void) fi;
	res=pwrite(fd, buf, size, offset);
	
		char folderpath[1024];
		char backup[1024]="/Backup";
		enkripsi(backup);
		char ekstensi[1024]=".ekstensi";
		char namafileawal[1024];
		snprintf(namafileawal,sizeof(namafileawal),"%s%s",folderpath,path);
		char namafile[1024];
		snprintf(namafile,sizeof(namafile),"/%s_",path);
		char namafilebackup[1024];
		char timestamp[1024];
		strcpy(folderpath,aslinya);
		strcat(folderpath,backup);
		
		if (stat(folderpath, &st) == -1) {
        mkdir(folderpath,0755);
    }
    
		time_t waktu =time(NULL);
		strftime(timestamp, 20 ,"%Y-%m-%d_%H:%M:%S", localtime(&waktu));
		strcpy(namafilebackup,namafile);
		strcat(namafilebackup,timestamp);
		strcat(namafilebackup,ekstensi);
		enkripsi(namafilebackup);
		strcat(folderpath,namafilebackup);
		rename(namafileawal,folderpath);
		
	close(fd);
	return res;

	int res,fd;
	char namafilesystem[1024];
	char fol[1024];
	snprintf(namafilesystem, sizeof(namafilesystem),"%s",path);
	enkripsi(namafilesystem);
	snprintf(fol,sizeof(fol),"%s%s",aslinya,namafilesystem);
}
```

Untuk folder RecycleBin:

```
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
```
