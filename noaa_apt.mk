##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=noaa_apt
ConfigurationName      :=Debug
WorkspacePath          := "/home/guest/Dokumenty/C-code/noaa_apt"
ProjectPath            := "/home/guest/Dokumenty/C-code/noaa_apt"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=
Date                   :=16.1.2019
CodeLitePath           :="/home/guest/.codelite"
LinkerName             :=gcc
SharedObjectLinkerName :=gcc -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="noaa_apt.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -O0
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)pthread $(LibrarySwitch)m $(LibrarySwitch)readline $(LibrarySwitch)termcap 
ArLibs                 :=  "pthread" "m" "readline" "termcap" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)Debug 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcu
CXX      := gcc
CC       := gcc
CXXFLAGS :=  -O3 -g -Wall $(Preprocessors)
CFLAGS   :=  -O3 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/main.c$(ObjectSuffix) $(IntermediateDirectory)/aptcode.c$(ObjectSuffix) $(IntermediateDirectory)/audioset.c$(ObjectSuffix) $(IntermediateDirectory)/image.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main.c$(ObjectSuffix): main.c $(IntermediateDirectory)/main.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/guest/Dokumenty/C-code/noaa_apt/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.c$(DependSuffix): main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.c$(ObjectSuffix) -MF$(IntermediateDirectory)/main.c$(DependSuffix) -MM "main.c"

$(IntermediateDirectory)/main.c$(PreprocessSuffix): main.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.c$(PreprocessSuffix) "main.c"

$(IntermediateDirectory)/aptcode.c$(ObjectSuffix): aptcode.c $(IntermediateDirectory)/aptcode.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/guest/Dokumenty/C-code/noaa_apt/aptcode.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/aptcode.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/aptcode.c$(DependSuffix): aptcode.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/aptcode.c$(ObjectSuffix) -MF$(IntermediateDirectory)/aptcode.c$(DependSuffix) -MM "aptcode.c"

$(IntermediateDirectory)/aptcode.c$(PreprocessSuffix): aptcode.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/aptcode.c$(PreprocessSuffix) "aptcode.c"

$(IntermediateDirectory)/audioset.c$(ObjectSuffix): audioset.c $(IntermediateDirectory)/audioset.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/guest/Dokumenty/C-code/noaa_apt/audioset.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/audioset.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/audioset.c$(DependSuffix): audioset.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/audioset.c$(ObjectSuffix) -MF$(IntermediateDirectory)/audioset.c$(DependSuffix) -MM "audioset.c"

$(IntermediateDirectory)/audioset.c$(PreprocessSuffix): audioset.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/audioset.c$(PreprocessSuffix) "audioset.c"

$(IntermediateDirectory)/image.c$(ObjectSuffix): image.c $(IntermediateDirectory)/image.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/guest/Dokumenty/C-code/noaa_apt/image.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/image.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/image.c$(DependSuffix): image.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/image.c$(ObjectSuffix) -MF$(IntermediateDirectory)/image.c$(DependSuffix) -MM "image.c"

$(IntermediateDirectory)/image.c$(PreprocessSuffix): image.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/image.c$(PreprocessSuffix) "image.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


