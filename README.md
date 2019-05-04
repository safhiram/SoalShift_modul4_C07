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

<h2>NOMOR 2</h2>
<p>
Semua file video yang tersimpan secara terpecah-pecah (splitted) harus secara otomatis tergabung (joined) dan diletakkan dalam folder “Videos”</p>
<p>Urutan operasi dari kebutuhan ini adalah:</p>
<p>* Tepat saat sebelum file system di-mount</p>
<p>* Secara otomatis folder “Videos” terbuat di root directory file system</p>
<p>* Misal ada sekumpulan file pecahan video bernama “computer.mkv.000”, “computer.mkv.001”, “computer.mkv.002”, dst. Maka secara otomatis file pecahan tersebut akan di-join menjadi file video “computer.mkv”</p>
<p>* Untuk mempermudah kalian, dipastikan hanya video file saja yang terpecah menjadi beberapa file. File pecahan tersebut dijamin terletak di root folder fuse</p>
<p>* Karena mungkin file video sangat banyak sehingga mungkin saja saat menggabungkan file video, file system akan membutuhkan waktu yang lama untuk sukses ter-mount. Maka pastikan saat akan menggabungkan file pecahan video, file system akan membuat 1 thread/proses(fork) baru yang dikhususkan untuk menggabungkan file video tersebut</p>
<p>* Pindahkan seluruh file video yang sudah ter-join ke dalam folder “Videos”</p>
<p>* Jangan tampilkan file pecahan di direktori manapun</p>
<p>* Tepat saat file system akan di-unmount</p>
<p>* Hapus semua file video yang berada di folder “Videos”, tapi jangan hapus file pecahan yang terdapat di root directory file system
<p>* Hapus folder “Videos”</p>
<h2>JAWABAN</h2>
Fungsi pre_init adalah fungsi yang dipanggil ketika file system akan di mount.
Awalnya membuat folder Videos. Lalu me-read nama file. Setiap file yang berakhiran .mkv maka lakukan penggabungan.

```
static void* pre_init(struct fuse_conn_info *conn)
{
        char folder[100000] = "/Videos";
		char folde1r[100000] = "/YOUTUBER";
		enc(folder);
		enc(folde1r);
		char fpath[1000];
    	sprintf(fpath,"%s%s", dirpath, folder);
		mkdir(fpath,0755);
		memset(fpath,0,sizeof(fpath));
		sprintf(fpath,"%s%s", dirpath, folde1r);
		mkdir(fpath,0755);
		memset(fpath,0,sizeof(fpath));

		pid_t child1;
		child1=fork();
		if(child1==0){
			DIR *dp;
			struct dirent *de;
			dp = opendir(dirpath);
			while((de = readdir(dp))){
				if(strcmp(de->d_name,".")!=0 && strcmp(de->d_name,"..")!=0){
					char ext[1000] = ".mkv";
					enc(ext);
					if(strlen(de->d_name)>7 && strncmp(de->d_name+strlen(de->d_name)-8,ext,4)==0){

							char joined[1000];
							char video[1000] = "/Videos";
							enc(video);
							sprintf(joined,"%s%s/",dirpath,video);
							strncat(joined,de->d_name,strlen(de->d_name)-4);
							FILE* mainj;
							mainj = fopen(joined,"a+");
							FILE* need;
							char this[1000];
							sprintf(this,"%s/%s",dirpath,de->d_name);
							need = fopen(this,"r");
							int c;
							while(1) {
   								c = fgetc(need);
   								if( feof(need) ) {
   								   break;
   								}
   								fprintf(mainj,"%c",c);
   							}
							
					}
				}
			}
			exit(EXIT_SUCCESS);
		}

        (void) conn;
        return NULL;
}

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
Untuk permasalahan , ketika kita mengedit suatu file maka akan dipanggil fungsi write.

1. Buat folder Backup yang bermode 0755, setelah itu write ulang isi dari file yang diedit dan ditaruh ke variabel buf
2. Jika file tersebut diedit, maka buat nama file nya dengan menyimpannya di string /Backup%s%s.ekstensi",name,time. Lalu isi file tersebut dengan isi dari variabel buf.

```
static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	int fd;
	int res;
    char fpath[1000];
    char name[1000];

	sprintf(name,"%s",path);
    enc(name);
	sprintf(fpath,"%s%s",dirpath,name);

	char newname[1000];
	char folder[1000] = "/Backup";
	enc(folder);
	char folderdir[1000];
	sprintf(folderdir,"%s%s",dirpath,folder);
	mkdir(folderdir,0755);

	(void) fi;
	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;
	printf("%s\n",buf);
	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);

	struct stat sd;
	if(stat(fpath,&sd)>-1){
		char t[1000];
		time_t now = time(NULL);
		strftime(t, 22, "_%Y-%m-%d_%H:%M:%S", localtime(&now));
		dec(name);
		sprintf(newname,"/Backup%s%s.ekstensi",name,t);
		enc(newname);
		memset(fpath,'\0',sizeof(fpath));
		sprintf(fpath,"%s%s",dirpath,newname);
		// printf("%s\n",fpath);
	
		FILE *fptr = fopen(fpath, "w+"); 
		fprintf(fptr,"%s",buf);
		fclose(fptr);

		return res;
	}
	
	return res;
}
}
```

Untuk permasalahan ketika kita menghapus suatu file , maka secara otomatis akan terpanggil fungsi unlink. Jika file tersebut didelete, buat folder bernama RecycleBin. Isi dari folder RecycleBin tersebut ada dua yaitu di file yang kita zip di file system aslinya, dan yang di foler backup.
Untuk membuat zip saat di file aslinya.
1. Deklarasikan string yang memuat namafile atau path file itu
2. Deklarasikan string yang memuat nama file zip nya.
3. Lakukan zip pada file tersebut dengan memberi nama seperti nama file zipnya dengan menggunakan thread
Untuk membuat zip saat di file Backup
1. Cocokkan nama file dengan nama file yang ada di folder backup. Jika nama file sama maka buat deklarasikan string yang memuat namafile atau path file itu dan eklarasikan string yang memuat nama file zip nya.
2. Lakukan zip pada file tersebut dengan memberi nama seperti nama file zipnya dengan menggunakan thread

```
static int xmp_unlink(const char *path)
{
	int res;
    char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
	if(strstr(name,".swp")==0 && strstr(name,".gooutpustream")==0){
		char folder[100000] = "/RecycleBin";
		enc(folder);
		char fpath[1000];
    	sprintf(fpath,"%s%s", dirpath, folder);
		mkdir(fpath,0755);

		char t[1000];
		time_t now = time(NULL);
		strftime(t, 22, "_%Y-%m-%d_%H:%M:%S", localtime(&now));

		char zip[1000];
		char fzip[1000];
		char fname[1000];
		memset(zip,0,sizeof(zip));
		memset(fzip,0,sizeof(fzip));
		memset(fname,0,sizeof(fname));
		sprintf(zip,"/RecycleBin%s_deleted_%s.zip",name,t);
		enc(name);
		sprintf(fname,"%s%s",dirpath,name);
		enc(zip);
		sprintf(fzip,"%s%s",dirpath,zip);
		pid_t child1;
		
		child1=fork();
		if(child1==0){
			execl("/usr/bin/zip","/usr/bin/zip","-q","-m","-j",fzip,fname,NULL);
			exit(EXIT_SUCCESS);
		}
		else{
			wait(NULL);
		}
		

		char fback[1000] = "/Backup";
		enc(fback);
		char foldbackp[1000];
		sprintf(foldbackp,"%s%s",dirpath,fback);
		DIR *dp;
		struct dirent *de;

		dp = opendir(foldbackp);
		while((de = readdir(dp))){
			if(strncmp(name+1,de->d_name,strlen(de->d_name)-29)==0){

				memset(fzip,0,sizeof(fzip));
				memset(zip,0,sizeof(zip));
				memset(fname,0,sizeof(fname));
				dec(name);
				sprintf(zip,"/RecycleBin%s_deleted_%s.zip",name,t);
				sprintf(fname,"%s/%s",foldbackp,de->d_name);
				enc(zip);
				enc(name);
				sprintf(fzip,"%s%s",dirpath,zip);

				child1=fork();
				if(child1==0){
					execl("/usr/bin/zip","/usr/bin/zip","-q","-m","-j","-u",fzip,fname,NULL);
					exit(EXIT_SUCCESS);
				}
				else{
					wait(NULL);
				}
			}
		}
		dec(name);
		memset(zip,0,sizeof(zip));
		memset(fzip,0,sizeof(fzip));
		memset(fname,0,sizeof(fname));
		sprintf(zip,"/RecycleBin%s_deleted_%s.zip",name,t);
		enc(name);
		sprintf(fname,"%s%s",dirpath,name);
		enc(zip);
		sprintf(fzip,"%s%s",dirpath,zip);
		memset(fname,0,sizeof(fname));
		strncpy(fname,fzip,strlen(fzip));
		strcat(fzip,".zip");
		rename(fzip,fname);
	}
	else{
    	enc(name);
		sprintf(fpath, "%s%s",dirpath,name);
		res = unlink(fpath);
		if (res == -1)
			return -errno;
	}
	return 0;
}

}
```
