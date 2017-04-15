// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>

#include <Psapi.h>

#using <System.dll>
using namespace System;
using namespace System::Diagnostics;
using namespace System::ComponentModel;

/*
int main()
{
	DWORD processId = GetCurrentProcessId();
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, false, processId);
	MEMORY_BASIC_INFORMATION* outProcess = new MEMORY_BASIC_INFORMATION();
	unsigned long regionIter = 0;
	size_t numBytes = VirtualQueryEx(process, (PVOID)regionIter, outProcess, sizeof(*outProcess));
	size_t biggestRegion = 0, currSize = outProcess->RegionSize;
	unsigned long allocBase = 0;
	unsigned long saveBiggestAllocBase = 0;
	bool firstTime = true;
	while (numBytes != 0)
	{
		if (allocBase == (ULONG)outProcess->AllocationBase && !firstTime)
		{
			currSize += outProcess->RegionSize;
		}
		else 
		{
			if (currSize > biggestRegion) 
			{
				biggestRegion = currSize;
				saveBiggestAllocBase = (ULONG)outProcess->AllocationBase;
			}
			currSize = 0;
		}
		firstTime = false;
		allocBase = (ULONG)(outProcess->AllocationBase) ;
		regionIter = outProcess->RegionSize + (ULONG)(outProcess->BaseAddress);
	
		numBytes = VirtualQueryEx(process, (PVOID)regionIter, outProcess, sizeof(*outProcess));
	}
	if (currSize > biggestRegion)
	{
		biggestRegion = currSize;
		saveBiggestAllocBase = allocBase;
	}
	LPTSTR outName = new TCHAR[1000];
	size_t len = GetModuleFileNameEx(process, false, outName, 1000);

	printf("Execulable Name: %s \n", outName);
	printf("Base Adress of Biggest Region: %lu \n", saveBiggestAllocBase);
	printf("Size of Biggest Region: %lu bytes\n", biggestRegion);


    return 0;
}
*/

void printBiggestReservedRegion(DWORD processId)
{
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, false, processId);
	MEMORY_BASIC_INFORMATION* outProcess = new MEMORY_BASIC_INFORMATION();
	unsigned long regionIter = 0;
	size_t numBytes = VirtualQueryEx(process, (PVOID)regionIter, outProcess, sizeof(*outProcess));
	size_t biggestRegion = 0, currSize = outProcess->RegionSize;
	unsigned long allocBase = 0;
	unsigned long saveBiggestAllocBase = 0;
	bool firstTime = true;
	while (numBytes != 0)
	{
		if (allocBase == (ULONG)outProcess->AllocationBase && !firstTime)
		{
			currSize += outProcess->RegionSize;
		}
		else
		{
			if (currSize > biggestRegion)
			{
				biggestRegion = currSize;
				saveBiggestAllocBase = (ULONG)outProcess->AllocationBase;
			}
			currSize = 0;
		}
		firstTime = false;
		allocBase = (ULONG)(outProcess->AllocationBase);
		regionIter = outProcess->RegionSize + (ULONG)(outProcess->BaseAddress);

		numBytes = VirtualQueryEx(process, (PVOID)regionIter, outProcess, sizeof(*outProcess));
	}
	if (currSize > biggestRegion)
	{
		biggestRegion = currSize;
		saveBiggestAllocBase = allocBase;
	}
	LPTSTR outName = new TCHAR[1000];
	size_t len = GetModuleFileNameEx(process, false, outName, 1000);
}

