#include <Windows.h>
#include <vector>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <direct.h>
#include <sys/stat.h>
#include <sys/types.h>

using std::string;

class FS {
public:
    static bool is_file(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return buf.st_mode & S_IFREG;
    }

    static bool is_dir(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return buf.st_mode & S_IFDIR;
    }

    static std::string GetBasePath() {
    char base_path[FILENAME_MAX];
    if (!_getcwd(base_path, sizeof(base_path))) {
        std::cout << "Error in 'getcwd'!\n";
        return "*";
    }

    return std::string(base_path);
    }

    static std::vector<std::string> GetFileNamesInDirectory(std::string directory) {
    directory.append("\\*");
    std::vector<std::string> files;
    WIN32_FIND_DATA fileData; 
    HANDLE hFind; 

    if ( !((hFind = FindFirstFile(directory.c_str(), &fileData)) == INVALID_HANDLE_VALUE) ) {
        while(FindNextFile(hFind, &fileData)) {
        if (std::string(fileData.cFileName).compare(".."))
            files.push_back(fileData.cFileName);
        }
    }
    
    FindClose(hFind);
    return files;
    }

    static std::string GetExt(std::string fname) {
    std::string ext = fname.substr(fname.find_last_of(".") + 1);
    if (ext.size() == fname.size()) return "";
    return ext;
    }
};

class MusicFolder {
private:
    std::vector<std::string> flist;
    std::vector<std::string> fnames;
    std::string pl_name;

public:
    MusicFolder (std::string fname) {
    std::cout << "Analyzing " << fname.c_str() << "\n";
    this->pl_name = fname;
    std::vector<std::string> vFileNames = FS::GetFileNamesInDirectory(fname);
    for (unsigned int i = 0; i < vFileNames.size(); ++i) {
        std::string name = fname;
        name.append("\\").append(vFileNames[i]);

        if (FS::is_file(name.c_str())) {
        std::cout << FS::GetExt(name).c_str() << " : " << vFileNames[i].c_str() << "\n";
        if (this->ext_is_valid(FS::GetExt(name))) {
            flist.push_back(name);
            fnames.push_back(vFileNames[i]);
        }
        }
    }  
    }

    void emit_wpl() {
    if (this->flist.size() == 0) {
        std::cout << this->pl_name.c_str() << " does not contain music!" << std::endl;
        return;
    }

    std::ofstream ofile;
    std::string oname = this->pl_name;
    oname.append(".wpl");
    ofile.open (oname, std::ios::out);
        
    ofile << "<?wpl version=\"1.0\"?>" << std::endl 
          << "<smil>" << std::endl
          << "  <head>" << std::endl
          << "    <meta name=\"Generator\" content=\"Microsoft Windows Media Player -- 12.0.7601.18741\"/>" << std::endl
          << "    <meta name=\"ItemCount\" content=\"" << this->flist.size() << "\"/>" << std::endl
          << "    <title>" << this->pl_name.c_str() << "</title>" << std::endl
          << "  </head>" << std::endl
              << "  <body>" << std::endl
              << "    <seq>" << std::endl;
    for (unsigned int i = 0; i < this->flist.size(); ++i) {
        ofile << "      <media src=\"" << flist[i].c_str() << "\"/>" << std::endl;
    }

    ofile << "    </seq>" << std::endl
          << "  </body>" << std::endl
          << "</smil>" << std::endl;
    
    ofile.close();
    }

    void emit_m3u() {
    if (this->flist.size() == 0) {
        std::cout << this->pl_name.c_str() << " does not contain music!" << std::endl;
        return;
    }

    std::ofstream ofile;
    std::string oname = this->pl_name;
    oname.append(".m3u");
    ofile.open (oname, std::ios::out);
        
    ofile << "#EXTM3U" << std::endl;

    for (unsigned int i = 0; i < this->flist.size(); ++i) {
        ofile << "#EXTINF:0," << fnames[i].c_str() << std::endl;
        ofile << flist[i].c_str() << std::endl << std::endl;
    }
    
    ofile.close();
    }

private:
    bool ext_is_valid (std::string ext) {
    if (ext.compare("mp3") == 0) return true;
    if (ext.compare("m4a") == 0) return true;

    return false;
    }
};

int main() {
    std::vector<std::string> vFileNames = FS::GetFileNamesInDirectory(".");
    for (unsigned int i = 0; i < vFileNames.size(); i++) {
    if (FS::is_dir(vFileNames[i].c_str())) {
        MusicFolder mf (vFileNames[i]);
        mf.emit_m3u();
    }
    }

    return 0;
}
