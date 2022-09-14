#include "Interpreter.h"

namespace Interpreter
{
	void Interpreter::EmuCop2()
	{
		///////////////////////////////////
		// COP2 Functions
		///////////////////////////////////

		//
		// NOT DOING THE FLAG SETTINGS
		//

		if (OpCode & 0x02000000)
		{
			switch (OpCode & 0x0000003F)
			{
			case 0x00000000: // "vaddx"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.CPR[VU_FS].X.f + VU0Regs.CPR[VU_FT].X.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.CPR[VU_FS].Y.f + VU0Regs.CPR[VU_FT].X.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.CPR[VU_FS].Z.f + VU0Regs.CPR[VU_FT].X.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.CPR[VU_FS].W.f + VU0Regs.CPR[VU_FT].X.f;
				}
				break;

			case 0x00000001: // "vaddy"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.CPR[VU_FS].X.f + VU0Regs.CPR[VU_FT].Y.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.CPR[VU_FS].Y.f + VU0Regs.CPR[VU_FT].Y.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.CPR[VU_FS].Z.f + VU0Regs.CPR[VU_FT].Y.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.CPR[VU_FS].W.f + VU0Regs.CPR[VU_FT].Y.f;
				}
				break;

			case 0x00000002: // "vaddz"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.CPR[VU_FS].X.f + VU0Regs.CPR[VU_FT].Z.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.CPR[VU_FS].Y.f + VU0Regs.CPR[VU_FT].Z.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.CPR[VU_FS].Z.f + VU0Regs.CPR[VU_FT].Z.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.CPR[VU_FS].W.f + VU0Regs.CPR[VU_FT].Z.f;
				}
				break;

			case 0x00000003: // "vaddw"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.CPR[VU_FS].X.f + VU0Regs.CPR[VU_FT].W.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.CPR[VU_FS].Y.f + VU0Regs.CPR[VU_FT].W.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.CPR[VU_FS].Z.f + VU0Regs.CPR[VU_FT].W.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.CPR[VU_FS].W.f + VU0Regs.CPR[VU_FT].W.f;
				}
				break;

			case 0x00000004: // "vsubx"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.CPR[VU_FS].X.f - VU0Regs.CPR[VU_FT].X.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.CPR[VU_FS].Y.f - VU0Regs.CPR[VU_FT].X.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.CPR[VU_FS].Z.f - VU0Regs.CPR[VU_FT].X.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.CPR[VU_FS].W.f - VU0Regs.CPR[VU_FT].X.f;
				}
				break;

			case 0x00000005: // "vsuby"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.CPR[VU_FS].X.f - VU0Regs.CPR[VU_FT].Y.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.CPR[VU_FS].Y.f - VU0Regs.CPR[VU_FT].Y.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.CPR[VU_FS].Z.f - VU0Regs.CPR[VU_FT].Y.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.CPR[VU_FS].W.f - VU0Regs.CPR[VU_FT].Y.f;
				}
				break;

			case 0x00000006: // "vsubz"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.CPR[VU_FS].X.f - VU0Regs.CPR[VU_FT].Z.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.CPR[VU_FS].Y.f - VU0Regs.CPR[VU_FT].Z.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.CPR[VU_FS].Z.f - VU0Regs.CPR[VU_FT].Z.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.CPR[VU_FS].W.f - VU0Regs.CPR[VU_FT].Z.f;
				}
				break;

			case 0x00000007: // "vsubw"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.CPR[VU_FS].X.f - VU0Regs.CPR[VU_FT].W.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.CPR[VU_FS].Y.f - VU0Regs.CPR[VU_FT].W.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.CPR[VU_FS].Z.f - VU0Regs.CPR[VU_FT].W.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.CPR[VU_FS].W.f - VU0Regs.CPR[VU_FT].W.f;
				}
				break;

			case 0x00000008: // "vmaddx"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.Acc.X.f + (VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].X.f);
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.Acc.Y.f + (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].X.f);
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.Acc.Z.f + (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].X.f);
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.Acc.W.f + (VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].X.f);
				}
				break;

			case 0x00000009: // "vmaddy"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.Acc.X.f + (VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].Y.f);
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.Acc.Y.f + (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].Y.f);
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.Acc.Z.f + (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].Y.f);
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.Acc.W.f + (VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].Y.f);
				}
				break;

			case 0x0000000A: // "vmaddz"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.Acc.X.f + (VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].Z.f);
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.Acc.Y.f + (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].Z.f);
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.Acc.Z.f + (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].Z.f);
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.Acc.W.f + (VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].Z.f);
				}
				break;

			case 0x0000000B: // "vmaddw"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.Acc.X.f + (VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].W.f);
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.Acc.Y.f + (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].W.f);
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.Acc.Z.f + (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].W.f);
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.Acc.W.f + (VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].W.f);
				}
				break;

			case 0x0000000C: // "vmsubx"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.Acc.X.f - (VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].X.f);
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.Acc.Y.f - (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].X.f);
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.Acc.Z.f - (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].X.f);
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.Acc.W.f - (VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].X.f);
				}
				break;

			case 0x0000000D: // "vmsuby"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.Acc.X.f - (VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].Y.f);
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.Acc.Y.f - (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].Y.f);
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.Acc.Z.f - (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].Y.f);
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.Acc.W.f - (VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].Y.f);
				}
				break;

			case 0x0000000E: // "vmsubz"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.Acc.X.f - (VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].Z.f);
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.Acc.Y.f - (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].Z.f);
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.Acc.Z.f - (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].Z.f);
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.Acc.W.f - (VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].Z.f);
				}
				break;

			case 0x0000000F: // "vmsubw"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.Acc.X.f - (VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].W.f);
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.Acc.Y.f - (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].W.f);
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.Acc.Z.f - (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].W.f);
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.Acc.W.f - (VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].W.f);
				}
				break;

			case 0x00000010: // "vmaxx"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = (VU0Regs.CPR[VU_FS].X.f > VU0Regs.CPR[VU_FT].X.f) ?
						VU0Regs.CPR[VU_FS].X.f : VU0Regs.CPR[VU_FT].X.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = (VU0Regs.CPR[VU_FS].Y.f > VU0Regs.CPR[VU_FT].X.f) ?
						VU0Regs.CPR[VU_FS].Y.f : VU0Regs.CPR[VU_FT].X.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = (VU0Regs.CPR[VU_FS].Z.f > VU0Regs.CPR[VU_FT].X.f) ?
						VU0Regs.CPR[VU_FS].Z.f : VU0Regs.CPR[VU_FT].X.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = (VU0Regs.CPR[VU_FS].W.f > VU0Regs.CPR[VU_FT].X.f) ?
						VU0Regs.CPR[VU_FS].W.f : VU0Regs.CPR[VU_FT].X.f;
				}
				break;

			case 0x00000011: // "vmaxy"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = (VU0Regs.CPR[VU_FS].X.f > VU0Regs.CPR[VU_FT].Y.f) ?
						VU0Regs.CPR[VU_FS].X.f : VU0Regs.CPR[VU_FT].Y.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = (VU0Regs.CPR[VU_FS].Y.f > VU0Regs.CPR[VU_FT].Y.f) ?
						VU0Regs.CPR[VU_FS].Y.f : VU0Regs.CPR[VU_FT].Y.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = (VU0Regs.CPR[VU_FS].Z.f > VU0Regs.CPR[VU_FT].Y.f) ?
						VU0Regs.CPR[VU_FS].Z.f : VU0Regs.CPR[VU_FT].Y.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = (VU0Regs.CPR[VU_FS].W.f > VU0Regs.CPR[VU_FT].Y.f) ?
						VU0Regs.CPR[VU_FS].W.f : VU0Regs.CPR[VU_FT].Y.f;
				}
				break;

			case 0x00000012: // "vmaxz"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = (VU0Regs.CPR[VU_FS].X.f > VU0Regs.CPR[VU_FT].Z.f) ?
						VU0Regs.CPR[VU_FS].X.f : VU0Regs.CPR[VU_FT].Z.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = (VU0Regs.CPR[VU_FS].Y.f > VU0Regs.CPR[VU_FT].Z.f) ?
						VU0Regs.CPR[VU_FS].Y.f : VU0Regs.CPR[VU_FT].Z.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = (VU0Regs.CPR[VU_FS].Z.f > VU0Regs.CPR[VU_FT].Z.f) ?
						VU0Regs.CPR[VU_FS].Z.f : VU0Regs.CPR[VU_FT].Z.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = (VU0Regs.CPR[VU_FS].W.f > VU0Regs.CPR[VU_FT].Z.f) ?
						VU0Regs.CPR[VU_FS].W.f : VU0Regs.CPR[VU_FT].Z.f;
				}
				break;

			case 0x00000013: // "vmaxw"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = (VU0Regs.CPR[VU_FS].X.f > VU0Regs.CPR[VU_FT].W.f) ?
						VU0Regs.CPR[VU_FS].X.f : VU0Regs.CPR[VU_FT].W.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = (VU0Regs.CPR[VU_FS].Y.f > VU0Regs.CPR[VU_FT].W.f) ?
						VU0Regs.CPR[VU_FS].Y.f : VU0Regs.CPR[VU_FT].W.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = (VU0Regs.CPR[VU_FS].Z.f > VU0Regs.CPR[VU_FT].W.f) ?
						VU0Regs.CPR[VU_FS].Z.f : VU0Regs.CPR[VU_FT].W.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = (VU0Regs.CPR[VU_FS].W.f > VU0Regs.CPR[VU_FT].W.f) ?
						VU0Regs.CPR[VU_FS].W.f : VU0Regs.CPR[VU_FT].W.f;
				}
				break;

			case 0x00000014: // "vminix"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = (VU0Regs.CPR[VU_FS].X.f < VU0Regs.CPR[VU_FT].X.f) ?
						VU0Regs.CPR[VU_FS].X.f : VU0Regs.CPR[VU_FT].X.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = (VU0Regs.CPR[VU_FS].Y.f < VU0Regs.CPR[VU_FT].X.f) ?
						VU0Regs.CPR[VU_FS].Y.f : VU0Regs.CPR[VU_FT].X.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = (VU0Regs.CPR[VU_FS].Z.f < VU0Regs.CPR[VU_FT].X.f) ?
						VU0Regs.CPR[VU_FS].Z.f : VU0Regs.CPR[VU_FT].X.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = (VU0Regs.CPR[VU_FS].W.f < VU0Regs.CPR[VU_FT].X.f) ?
						VU0Regs.CPR[VU_FS].W.f : VU0Regs.CPR[VU_FT].X.f;
				}
				break;

			case 0x00000015: // "vminiy"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = (VU0Regs.CPR[VU_FS].X.f < VU0Regs.CPR[VU_FT].Y.f) ?
						VU0Regs.CPR[VU_FS].X.f : VU0Regs.CPR[VU_FT].Y.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = (VU0Regs.CPR[VU_FS].Y.f < VU0Regs.CPR[VU_FT].Y.f) ?
						VU0Regs.CPR[VU_FS].Y.f : VU0Regs.CPR[VU_FT].Y.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = (VU0Regs.CPR[VU_FS].Z.f < VU0Regs.CPR[VU_FT].Y.f) ?
						VU0Regs.CPR[VU_FS].Z.f : VU0Regs.CPR[VU_FT].Y.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = (VU0Regs.CPR[VU_FS].W.f < VU0Regs.CPR[VU_FT].Y.f) ?
						VU0Regs.CPR[VU_FS].W.f : VU0Regs.CPR[VU_FT].Y.f;
				}
				break;

			case 0x00000016: // "vminiz"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = (VU0Regs.CPR[VU_FS].X.f < VU0Regs.CPR[VU_FT].Z.f) ?
						VU0Regs.CPR[VU_FS].X.f : VU0Regs.CPR[VU_FT].Z.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = (VU0Regs.CPR[VU_FS].Y.f < VU0Regs.CPR[VU_FT].Z.f) ?
						VU0Regs.CPR[VU_FS].Y.f : VU0Regs.CPR[VU_FT].Z.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = (VU0Regs.CPR[VU_FS].Z.f < VU0Regs.CPR[VU_FT].Z.f) ?
						VU0Regs.CPR[VU_FS].Z.f : VU0Regs.CPR[VU_FT].Z.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = (VU0Regs.CPR[VU_FS].W.f < VU0Regs.CPR[VU_FT].Z.f) ?
						VU0Regs.CPR[VU_FS].W.f : VU0Regs.CPR[VU_FT].Z.f;
				}
				break;

			case 0x00000017: // "vminiw"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = (VU0Regs.CPR[VU_FS].X.f < VU0Regs.CPR[VU_FT].W.f) ?
						VU0Regs.CPR[VU_FS].X.f : VU0Regs.CPR[VU_FT].W.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = (VU0Regs.CPR[VU_FS].Y.f < VU0Regs.CPR[VU_FT].W.f) ?
						VU0Regs.CPR[VU_FS].Y.f : VU0Regs.CPR[VU_FT].W.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = (VU0Regs.CPR[VU_FS].Z.f < VU0Regs.CPR[VU_FT].W.f) ?
						VU0Regs.CPR[VU_FS].Z.f : VU0Regs.CPR[VU_FT].W.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = (VU0Regs.CPR[VU_FS].W.f < VU0Regs.CPR[VU_FT].W.f) ?
						VU0Regs.CPR[VU_FS].W.f : VU0Regs.CPR[VU_FT].W.f;
				}
				break;

			case 0x00000018: // "vmulx"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].X.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].X.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].X.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].X.f;
				}
				break;

			case 0x00000019: // "vmuly"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].Y.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].Y.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].Y.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].Y.f;
				}
				break;

			case 0x0000001A: // "vmulz"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].Z.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].Z.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].Z.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].Z.f;
				}
				break;

			case 0x0000001B: // "vmulw"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].W.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].W.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].W.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].W.f;
				}
				break;

			case 0x0000001C: // "vmulq"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.CPR[VU_FS].X.f * VU0Regs.Q.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.CPR[VU_FS].Y.f * VU0Regs.Q.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.CPR[VU_FS].Z.f * VU0Regs.Q.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.CPR[VU_FS].W.f * VU0Regs.Q.f;
				}
				break;

			case 0x0000001D: // "vmaxi"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = (VU0Regs.CPR[VU_FS].X.f > VU0Regs.I.f) ?
						VU0Regs.CPR[VU_FS].X.f : VU0Regs.I.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = (VU0Regs.CPR[VU_FS].Y.f > VU0Regs.I.f) ?
						VU0Regs.CPR[VU_FS].Y.f : VU0Regs.I.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = (VU0Regs.CPR[VU_FS].Z.f > VU0Regs.I.f) ?
						VU0Regs.CPR[VU_FS].Z.f : VU0Regs.I.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = (VU0Regs.CPR[VU_FS].W.f > VU0Regs.I.f) ?
						VU0Regs.CPR[VU_FS].W.f : VU0Regs.I.f;
				}
				break;

			case 0x0000001E: // "vmuli"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.CPR[VU_FS].X.f * VU0Regs.I.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.CPR[VU_FS].Y.f * VU0Regs.I.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.CPR[VU_FS].Z.f * VU0Regs.I.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.CPR[VU_FS].W.f * VU0Regs.I.f;
				}
				break;

			case 0x0000001F: // "vminii"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = (VU0Regs.CPR[VU_FS].X.f < VU0Regs.I.f) ?
						VU0Regs.CPR[VU_FS].X.f : VU0Regs.I.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = (VU0Regs.CPR[VU_FS].Y.f < VU0Regs.I.f) ?
						VU0Regs.CPR[VU_FS].Y.f : VU0Regs.I.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = (VU0Regs.CPR[VU_FS].Z.f < VU0Regs.I.f) ?
						VU0Regs.CPR[VU_FS].Z.f : VU0Regs.I.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = (VU0Regs.CPR[VU_FS].W.f < VU0Regs.I.f) ?
						VU0Regs.CPR[VU_FS].W.f : VU0Regs.I.f;
				}
				break;

			case 0x00000020: // "vaddq"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.CPR[VU_FS].X.f + VU0Regs.Q.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.CPR[VU_FS].Y.f + VU0Regs.Q.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.CPR[VU_FS].Z.f + VU0Regs.Q.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.CPR[VU_FS].W.f + VU0Regs.Q.f;
				}
				break;

			case 0x00000021: // "vmaddq"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.Acc.X.f + (VU0Regs.CPR[VU_FS].X.f * VU0Regs.Q.f);
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.Acc.Y.f + (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.Q.f);
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.Acc.Z.f + (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.Q.f);
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.Acc.W.f + (VU0Regs.CPR[VU_FS].W.f * VU0Regs.Q.f);
				}
				break;

			case 0x00000022: // "vaddi"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.CPR[VU_FS].X.f + VU0Regs.I.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.CPR[VU_FS].Y.f + VU0Regs.I.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.CPR[VU_FS].Z.f + VU0Regs.I.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.CPR[VU_FS].W.f + VU0Regs.I.f;
				}
				break;

			case 0x00000023: // "vmaddi"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.Acc.X.f + (VU0Regs.CPR[VU_FS].X.f * VU0Regs.I.f);
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.Acc.Y.f + (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.I.f);
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.Acc.Z.f + (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.I.f);
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.Acc.W.f + (VU0Regs.CPR[VU_FS].W.f * VU0Regs.I.f);
				}
				break;

			case 0x00000024: // "vsubq"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.CPR[VU_FS].X.f - VU0Regs.Q.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.CPR[VU_FS].Y.f - VU0Regs.Q.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.CPR[VU_FS].Z.f - VU0Regs.Q.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.CPR[VU_FS].W.f - VU0Regs.Q.f;
				}
				break;

			case 0x00000025: // "vmsubq"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.Acc.X.f - (VU0Regs.CPR[VU_FS].X.f * VU0Regs.Q.f);
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.Acc.Y.f - (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.Q.f);
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.Acc.Z.f - (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.Q.f);
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.Acc.W.f - (VU0Regs.CPR[VU_FS].W.f * VU0Regs.Q.f);
				}
				break;

			case 0x00000026: // "vsubi"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.CPR[VU_FS].X.f - VU0Regs.I.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.CPR[VU_FS].Y.f - VU0Regs.I.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.CPR[VU_FS].Z.f - VU0Regs.I.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.CPR[VU_FS].W.f - VU0Regs.I.f;
				}
				break;

			case 0x00000027: // "vmsubi"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.Acc.X.f - (VU0Regs.CPR[VU_FS].X.f * VU0Regs.I.f);
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.Acc.Y.f - (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.I.f);
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.Acc.Z.f - (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.I.f);
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.Acc.W.f - (VU0Regs.CPR[VU_FS].W.f * VU0Regs.I.f);
				}
				break;

			case 0x00000028: // "vadd"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.CPR[VU_FS].X.f + VU0Regs.CPR[VU_FT].X.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.CPR[VU_FS].Y.f + VU0Regs.CPR[VU_FT].Y.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.CPR[VU_FS].Z.f + VU0Regs.CPR[VU_FT].Z.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.CPR[VU_FS].W.f + VU0Regs.CPR[VU_FT].W.f;
				}
				break;

			case 0x00000029: // "vmadd"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.Acc.X.f + (VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].X.f);
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.Acc.Y.f + (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].Y.f);
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.Acc.Z.f + (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].Z.f);
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.Acc.W.f + (VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].W.f);
				}
				break;

			case 0x0000002A: // "vmul"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].X.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].Y.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].Z.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].W.f;
				}
				break;

			case 0x0000002B: // "vmax"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = (VU0Regs.CPR[VU_FS].X.f > VU0Regs.CPR[VU_FT].X.f) ?
						VU0Regs.CPR[VU_FS].X.f : VU0Regs.CPR[VU_FT].X.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = (VU0Regs.CPR[VU_FS].Y.f > VU0Regs.CPR[VU_FT].Y.f) ?
						VU0Regs.CPR[VU_FS].Y.f : VU0Regs.CPR[VU_FT].Y.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = (VU0Regs.CPR[VU_FS].Z.f > VU0Regs.CPR[VU_FT].Z.f) ?
						VU0Regs.CPR[VU_FS].Z.f : VU0Regs.CPR[VU_FT].Z.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = (VU0Regs.CPR[VU_FS].W.f > VU0Regs.CPR[VU_FT].W.f) ?
						VU0Regs.CPR[VU_FS].W.f : VU0Regs.CPR[VU_FT].W.f;
				}
				break;

			case 0x0000002C: // "vsub"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.CPR[VU_FS].X.f - VU0Regs.CPR[VU_FT].X.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.CPR[VU_FS].Y.f - VU0Regs.CPR[VU_FT].Y.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.CPR[VU_FS].Z.f - VU0Regs.CPR[VU_FT].Z.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.CPR[VU_FS].W.f - VU0Regs.CPR[VU_FT].W.f;
				}
				break;

			case 0x0000002D: // "vmsub"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = VU0Regs.Acc.X.f - (VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].X.f);
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = VU0Regs.Acc.Y.f - (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].Y.f);
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = VU0Regs.Acc.Z.f - (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].Z.f);
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = VU0Regs.Acc.W.f - (VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].W.f);
				}
				break;

			case 0x0000002E: // "vopmsub"
				VU0Regs.CPR[VU_FD].X.f = VU0Regs.Acc.X.f - (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].Z.f);
				VU0Regs.CPR[VU_FD].Y.f = VU0Regs.Acc.Y.f - (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].X.f);
				VU0Regs.CPR[VU_FD].Z.f = VU0Regs.Acc.Z.f - (VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].Y.f);
				break;

			case 0x0000002F: // "vmini"
				if (VU_DEST_X)
				{
					VU0Regs.CPR[VU_FD].X.f = (VU0Regs.CPR[VU_FS].X.f < VU0Regs.CPR[VU_FT].X.f) ?
						VU0Regs.CPR[VU_FS].X.f : VU0Regs.CPR[VU_FT].X.f;
				}
				if (VU_DEST_Y)
				{
					VU0Regs.CPR[VU_FD].Y.f = (VU0Regs.CPR[VU_FS].Y.f < VU0Regs.CPR[VU_FT].Y.f) ?
						VU0Regs.CPR[VU_FS].Y.f : VU0Regs.CPR[VU_FT].Y.f;
				}
				if (VU_DEST_Z)
				{
					VU0Regs.CPR[VU_FD].Z.f = (VU0Regs.CPR[VU_FS].Z.f < VU0Regs.CPR[VU_FT].Z.f) ?
						VU0Regs.CPR[VU_FS].Z.f : VU0Regs.CPR[VU_FT].Z.f;
				}
				if (VU_DEST_W)
				{
					VU0Regs.CPR[VU_FD].W.f = (VU0Regs.CPR[VU_FS].W.f < VU0Regs.CPR[VU_FT].W.f) ?
						VU0Regs.CPR[VU_FS].W.f : VU0Regs.CPR[VU_FT].W.f;
				}
				break;

			case 0x00000030: // "viadd"
				VU0Regs.CCR[VU_ID].u16_00_15 = VU0Regs.CCR[VU_IS].u16_00_15 + VU0Regs.CCR[VU_IT].u16_00_15;
				break;

			case 0x00000031: // "visub"
				VU0Regs.CCR[VU_ID].u16_00_15 = VU0Regs.CCR[VU_IS].u16_00_15 - VU0Regs.CCR[VU_IT].u16_00_15;
				break;

			case 0x00000032: // "viaddi"
				VU0Regs.CCR[VU_IT].u16_00_15 = VU0Regs.CCR[VU_IS].u16_00_15 + VU_IMM5;
				break;

			case 0x00000034: // "viand"
				VU0Regs.CCR[VU_ID].u16_00_15 = VU0Regs.CCR[VU_IS].u16_00_15 & VU0Regs.CCR[VU_IT].u16_00_15;
				break;

			case 0x00000035: // "vior"
				VU0Regs.CCR[VU_ID].u16_00_15 = VU0Regs.CCR[VU_IS].u16_00_15 | VU0Regs.CCR[VU_IT].u16_00_15;
				break;

			case 0x00000038: // "vcallms"
				break;

			case 0x00000039: // "vcallmsr"
				break;

			default:
				switch (OpCode & 0x000007FF)
				{
				case 0x0000003C: // "vaddax" bc
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.CPR[VU_FS].X.f + VU0Regs.CPR[VU_FT].X.f;
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.CPR[VU_FS].Y.f + VU0Regs.CPR[VU_FT].X.f;
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.CPR[VU_FS].Z.f + VU0Regs.CPR[VU_FT].X.f;
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.CPR[VU_FS].W.f + VU0Regs.CPR[VU_FT].X.f;
					}
					break;

				case 0x0000003D: // "vadday" bc 
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.CPR[VU_FS].X.f + VU0Regs.CPR[VU_FT].Y.f;
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.CPR[VU_FS].Y.f + VU0Regs.CPR[VU_FT].Y.f;
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.CPR[VU_FS].Z.f + VU0Regs.CPR[VU_FT].Y.f;
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.CPR[VU_FS].W.f + VU0Regs.CPR[VU_FT].Y.f;
					}
					break;

				case 0x0000003E: // "vaddaz" bc
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.CPR[VU_FS].X.f + VU0Regs.CPR[VU_FT].Z.f;
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.CPR[VU_FS].Y.f + VU0Regs.CPR[VU_FT].Z.f;
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.CPR[VU_FS].Z.f + VU0Regs.CPR[VU_FT].Z.f;
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.CPR[VU_FS].W.f + VU0Regs.CPR[VU_FT].Z.f;
					}
					break;

				case 0x0000003F: // "vaddaw" bc
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.CPR[VU_FS].X.f + VU0Regs.CPR[VU_FT].W.f;
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.CPR[VU_FS].Y.f + VU0Regs.CPR[VU_FT].W.f;
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.CPR[VU_FS].Z.f + VU0Regs.CPR[VU_FT].W.f;
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.CPR[VU_FS].W.f + VU0Regs.CPR[VU_FT].W.f;
					}
					break;

				case 0x0000007C: // "vsubax" bc
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.CPR[VU_FS].X.f - VU0Regs.CPR[VU_FT].X.f;
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.CPR[VU_FS].Y.f - VU0Regs.CPR[VU_FT].X.f;
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.CPR[VU_FS].Z.f - VU0Regs.CPR[VU_FT].X.f;
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.CPR[VU_FS].W.f - VU0Regs.CPR[VU_FT].X.f;
					}
					break;

				case 0x0000007D: // "vsubay" bc
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.CPR[VU_FS].X.f - VU0Regs.CPR[VU_FT].Y.f;
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.CPR[VU_FS].Y.f - VU0Regs.CPR[VU_FT].Y.f;
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.CPR[VU_FS].Z.f - VU0Regs.CPR[VU_FT].Y.f;
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.CPR[VU_FS].W.f - VU0Regs.CPR[VU_FT].Y.f;
					}
					break;

				case 0x0000007E: // "vsubaz" bc
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.CPR[VU_FS].X.f - VU0Regs.CPR[VU_FT].Z.f;
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.CPR[VU_FS].Y.f - VU0Regs.CPR[VU_FT].Z.f;
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.CPR[VU_FS].Z.f - VU0Regs.CPR[VU_FT].Z.f;
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.CPR[VU_FS].W.f - VU0Regs.CPR[VU_FT].Z.f;
					}
					break;

				case 0x0000007F: // "vsubaw" bc
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.CPR[VU_FS].X.f - VU0Regs.CPR[VU_FT].W.f;
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.CPR[VU_FS].Y.f - VU0Regs.CPR[VU_FT].W.f;
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.CPR[VU_FS].Z.f - VU0Regs.CPR[VU_FT].W.f;
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.CPR[VU_FS].W.f - VU0Regs.CPR[VU_FT].W.f;
					}
					break;

				case 0x000000BC: // "vmaddax" bc
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.Acc.X.f + (VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].X.f);
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.Acc.Y.f + (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].X.f);
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.Acc.Z.f + (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].X.f);
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.Acc.W.f + (VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].X.f);
					}
					break;

				case 0x000000BD: // "vmadday" bc
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.Acc.X.f + (VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].Y.f);
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.Acc.Y.f + (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].Y.f);
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.Acc.Z.f + (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].Y.f);
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.Acc.W.f + (VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].Y.f);
					}
					break;

				case 0x000000BE: // "vmaddaz" bc
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.Acc.X.f + (VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].Z.f);
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.Acc.Y.f + (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].Z.f);
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.Acc.Z.f + (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].Z.f);
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.Acc.W.f + (VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].Z.f);
					}
					break;

				case 0x000000BF: // "vmaddaw" bc
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.Acc.X.f + (VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].W.f);
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.Acc.Y.f + (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].W.f);
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.Acc.Z.f + (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].W.f);
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.Acc.W.f + (VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].W.f);
					}
					break;

				case 0x000000FC: // "vmsubax" bc
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.Acc.X.f - (VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].X.f);
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.Acc.Y.f - (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].X.f);
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.Acc.Z.f - (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].X.f);
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.Acc.W.f - (VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].X.f);
					}
					break;

				case 0x000000FD: // "vmsubay" bc
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.Acc.X.f - (VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].Y.f);
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.Acc.Y.f - (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].Y.f);
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.Acc.Z.f - (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].Y.f);
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.Acc.W.f - (VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].Y.f);
					}
					break;

				case 0x000000FE: // "vmsubaz" bc
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.Acc.X.f - (VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].Z.f);
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.Acc.Y.f - (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].Z.f);
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.Acc.Z.f - (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].Z.f);
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.Acc.W.f - (VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].Z.f);
					}
					break;

				case 0x000000FF: // "vmsubaw" bc
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.Acc.X.f - (VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].W.f);
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.Acc.Y.f - (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].W.f);
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.Acc.Z.f - (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].W.f);
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.Acc.W.f - (VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].W.f);
					}
					break;

				case 0x0000013C: // "vitof0"
					if (VU_DEST_X)
					{
						VU0Regs.CPR[VU_FT].X.f = *(EMU_I32*)&VU0Regs.CPR[VU_FS].X.i;
					}
					if (VU_DEST_Y)
					{
						VU0Regs.CPR[VU_FT].Y.f = *(EMU_I32*)&VU0Regs.CPR[VU_FS].Y.i;
					}
					if (VU_DEST_Z)
					{
						VU0Regs.CPR[VU_FT].Z.f = *(EMU_I32*)&VU0Regs.CPR[VU_FS].Z.i;
					}
					if (VU_DEST_W)
					{
						VU0Regs.CPR[VU_FT].W.f = *(EMU_I32*)&VU0Regs.CPR[VU_FS].W.i;
					}
					break;

				case 0x0000013D: // "vitof4"
					if (VU_DEST_X)
					{
						VU0Regs.CPR[VU_FT].X.f = (float)((double)(*(EMU_I32*)&VU0Regs.CPR[VU_FS].X.f) * 0.0625f);
					}
					if (VU_DEST_Y)
					{
						VU0Regs.CPR[VU_FT].Y.f = (float)((double)(*(EMU_I32*)&VU0Regs.CPR[VU_FS].Y.f) * 0.0625f);
					}
					if (VU_DEST_Z)
					{
						VU0Regs.CPR[VU_FT].Z.f = (float)((double)(*(EMU_I32*)&VU0Regs.CPR[VU_FS].Z.f) * 0.0625f);
					}
					if (VU_DEST_W)
					{
						VU0Regs.CPR[VU_FT].W.f = (float)((double)(*(EMU_I32*)&VU0Regs.CPR[VU_FS].W.f) * 0.0625f);
					}
					break;

				case 0x0000013E: // "vitof12"
					if (VU_DEST_X)
					{
						VU0Regs.CPR[VU_FT].X.f = (float)((double)(*(EMU_I32*)&VU0Regs.CPR[VU_FS].X.f) * 0.000244140625f);
					}
					if (VU_DEST_Y)
					{
						VU0Regs.CPR[VU_FT].Y.f = (float)((double)(*(EMU_I32*)&VU0Regs.CPR[VU_FS].Y.f) * 0.000244140625f);
					}
					if (VU_DEST_Z)
					{
						VU0Regs.CPR[VU_FT].Z.f = (float)((double)(*(EMU_I32*)&VU0Regs.CPR[VU_FS].Z.f) * 0.000244140625f);
					}
					if (VU_DEST_W)
					{
						VU0Regs.CPR[VU_FT].W.f = (float)((double)(*(EMU_I32*)&VU0Regs.CPR[VU_FS].W.f) * 0.000244140625f);
					}
					break;

				case 0x0000013F: // "vitof15"
					if (VU_DEST_X)
					{
						VU0Regs.CPR[VU_FT].X.f = (float)((double)(*(EMU_I32*)&VU0Regs.CPR[VU_FS].X.f) * 0.000030517578125);
					}
					if (VU_DEST_Y)
					{
						VU0Regs.CPR[VU_FT].Y.f = (float)((double)(*(EMU_I32*)&VU0Regs.CPR[VU_FS].Y.f) * 0.000030517578125);
					}
					if (VU_DEST_Z)
					{
						VU0Regs.CPR[VU_FT].Z.f = (float)((double)(*(EMU_I32*)&VU0Regs.CPR[VU_FS].Z.f) * 0.000030517578125);
					}
					if (VU_DEST_W)
					{
						VU0Regs.CPR[VU_FT].W.f = (float)((double)(*(EMU_I32*)&VU0Regs.CPR[VU_FS].W.f) * 0.000030517578125);
					}
					break;

				case 0x0000017C: // "vftoi0"
					if (VU_DEST_X)
					{
						*(EMU_I32*)&VU0Regs.CPR[VU_FT].X.f = (EMU_I32)VU0Regs.CPR[VU_FS].X.f;
					}
					if (VU_DEST_Y)
					{
						*(EMU_I32*)&VU0Regs.CPR[VU_FT].Y.f = (EMU_I32)VU0Regs.CPR[VU_FS].Y.f;
					}
					if (VU_DEST_Z)
					{
						*(EMU_I32*)&VU0Regs.CPR[VU_FT].Z.f = (EMU_I32)VU0Regs.CPR[VU_FS].Z.f;
					}
					if (VU_DEST_W)
					{
						*(EMU_I32*)&VU0Regs.CPR[VU_FT].W.f = (EMU_I32)VU0Regs.CPR[VU_FS].W.f;
					}
					break;

				case 0x0000017D: // "vftoi4"
					if (VU_DEST_X)
					{
						*(EMU_I32*)&VU0Regs.CPR[VU_FT].X.f = (EMU_I32)((double)VU0Regs.CPR[VU_FS].X.f / 0.0625);
					}
					if (VU_DEST_Y)
					{
						*(EMU_I32*)&VU0Regs.CPR[VU_FT].Y.f = (EMU_I32)((double)VU0Regs.CPR[VU_FS].Y.f / 0.0625);
					}
					if (VU_DEST_Z)
					{
						*(EMU_I32*)&VU0Regs.CPR[VU_FT].Z.f = (EMU_I32)((double)VU0Regs.CPR[VU_FS].Z.f / 0.0625);
					}
					if (VU_DEST_W)
					{
						*(EMU_I32*)&VU0Regs.CPR[VU_FT].W.f = (EMU_I32)((double)VU0Regs.CPR[VU_FS].W.f / 0.0625);
					}
					break;

				case 0x0000017E: // "vftoi12"
					if (VU_DEST_X)
					{
						*(EMU_I32*)&VU0Regs.CPR[VU_FT].X.f = (EMU_I32)((double)VU0Regs.CPR[VU_FS].X.f / 0.000244140625);
					}
					if (VU_DEST_Y)
					{
						*(EMU_I32*)&VU0Regs.CPR[VU_FT].Y.f = (EMU_I32)((double)VU0Regs.CPR[VU_FS].Y.f / 0.000244140625);
					}
					if (VU_DEST_Z)
					{
						*(EMU_I32*)&VU0Regs.CPR[VU_FT].Z.f = (EMU_I32)((double)VU0Regs.CPR[VU_FS].Z.f / 0.000244140625);
					}
					if (VU_DEST_W)
					{
						*(EMU_I32*)&VU0Regs.CPR[VU_FT].W.f = (EMU_I32)((double)VU0Regs.CPR[VU_FS].W.f / 0.000244140625);
					}
					break;

				case 0x0000017F: // "vftoi15"
					if (VU_DEST_X)
					{
						*(EMU_I32*)&VU0Regs.CPR[VU_FT].X.f = (EMU_I32)((double)VU0Regs.CPR[VU_FS].X.f / 0.000030517578125);
					}
					if (VU_DEST_Y)
					{
						*(EMU_I32*)&VU0Regs.CPR[VU_FT].Y.f = (EMU_I32)((double)VU0Regs.CPR[VU_FS].Y.f / 0.000030517578125);
					}
					if (VU_DEST_Z)
					{
						*(EMU_I32*)&VU0Regs.CPR[VU_FT].Z.f = (EMU_I32)((double)VU0Regs.CPR[VU_FS].Z.f / 0.000030517578125);
					}
					if (VU_DEST_W)
					{
						*(EMU_I32*)&VU0Regs.CPR[VU_FT].W.f = (EMU_I32)((double)VU0Regs.CPR[VU_FS].W.f / 0.000030517578125);
					}
					break;

				case 0x000001BC: // "vmulax" bc
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].X.f;
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].X.f;
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].X.f;
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].X.f;
					}
					break;

				case 0x000001BD: // "vmulay" bc
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].Y.f;
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].Y.f;
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].Y.f;
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].Y.f;
					}
					break;

				case 0x000001BE: // "vmulaz" bc
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].Z.f;
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].Z.f;
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].Z.f;
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].Z.f;
					}
					break;

				case 0x000001BF: // "vmulaw" bc
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].W.f;
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].W.f;
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].W.f;
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].W.f;
					}
					break;

				case 0x000001FC: // "vmulaq"
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.CPR[VU_FS].X.f * VU0Regs.Q.f;
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.CPR[VU_FS].Y.f * VU0Regs.Q.f;
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.CPR[VU_FS].Z.f * VU0Regs.Q.f;
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.CPR[VU_FS].W.f * VU0Regs.Q.f;
					}
					break;

				case 0x000001FD: // "vabs"
					if (VU_DEST_X)
					{
						VU0Regs.CPR[VU_FT].X.f = (float)fabs(VU0Regs.CPR[VU_FS].X.f);
					}
					if (VU_DEST_Y)
					{
						VU0Regs.CPR[VU_FT].Y.f = (float)fabs(VU0Regs.CPR[VU_FS].Y.f);
					}
					if (VU_DEST_Z)
					{
						VU0Regs.CPR[VU_FT].Z.f = (float)fabs(VU0Regs.CPR[VU_FS].Z.f);
					}
					if (VU_DEST_W)
					{
						VU0Regs.CPR[VU_FT].W.f = (float)fabs(VU0Regs.CPR[VU_FS].W.f);
					}
					break;

				case 0x000001FE: // "vmulai"
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.CPR[VU_FS].X.f * VU0Regs.I.f;
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.CPR[VU_FS].Y.f * VU0Regs.I.f;
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.CPR[VU_FS].Z.f * VU0Regs.I.f;
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.CPR[VU_FS].W.f * VU0Regs.I.f;
					}
					break;
					/////////////////////////////////////////////////////////////////////////////////////////////
				case 0x000001FF: // "vclipw.xyz"
					VU0Regs.Clipping = VU0Regs.Clipping << 6;

					if (VU0Regs.CPR[VU_FS].X.f > fabs(VU0Regs.CPR[VU_FT].W.f))
					{
						VU0Regs.Clipping_1HX = 1;
					}
					else
					{
						// Is that right ?????
						VU0Regs.Clipping_1HX = 0;
					}
					if (VU0Regs.CPR[VU_FS].X.f < -(fabs(VU0Regs.CPR[VU_FT].W.f)))
					{
						VU0Regs.Clipping_1LX = 1;
					}
					else
					{
						// Is that right ?????
						VU0Regs.Clipping_1LX = 0;
					}
					if (VU0Regs.CPR[VU_FS].Y.f > fabs(VU0Regs.CPR[VU_FT].W.f))
					{
						VU0Regs.Clipping_1HY = 1;
					}
					else
					{
						// Is that right ?????
						VU0Regs.Clipping_1HY = 0;
					}
					if (VU0Regs.CPR[VU_FS].Y.f < -(fabs(VU0Regs.CPR[VU_FT].W.f)))
					{
						VU0Regs.Clipping_1LY = 1;
					}
					else
					{
						// Is that right ?????
						VU0Regs.Clipping_1LY = 0;
					}
					if (VU0Regs.CPR[VU_FS].Z.f > fabs(VU0Regs.CPR[VU_FT].W.f))
					{
						VU0Regs.Clipping_1HZ = 1;
					}
					else
					{
						// Is that right ?????
						VU0Regs.Clipping_1HZ = 0;
					}
					if (VU0Regs.CPR[VU_FS].Z.f < -(fabs(VU0Regs.CPR[VU_FT].W.f)))
					{
						VU0Regs.Clipping_1LZ = 1;
					}
					else
					{
						// Is that right ?????
						VU0Regs.Clipping_1LZ = 0;
					}

					break;

				case 0x0000023C: // "vaddaq"
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.CPR[VU_FS].X.f + VU0Regs.Q.f;
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.CPR[VU_FS].Y.f + VU0Regs.Q.f;
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.CPR[VU_FS].Z.f + VU0Regs.Q.f;
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.CPR[VU_FS].W.f + VU0Regs.Q.f;
					}
					break;

				case 0x0000023D: // "vmaddaq"
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.Acc.X.f + (VU0Regs.CPR[VU_FS].X.f * VU0Regs.Q.f);
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.Acc.Y.f + (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.Q.f);
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.Acc.Z.f + (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.Q.f);
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.Acc.W.f + (VU0Regs.CPR[VU_FS].W.f * VU0Regs.Q.f);
					}
					break;

				case 0x0000023E: // "vaddai"
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.CPR[VU_FS].X.f + VU0Regs.I.f;
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.CPR[VU_FS].Y.f + VU0Regs.I.f;
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.CPR[VU_FS].Z.f + VU0Regs.I.f;
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.CPR[VU_FS].W.f + VU0Regs.I.f;
					}
					break;

				case 0x0000023F: // "vmaddai"
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.Acc.X.f + (VU0Regs.CPR[VU_FS].X.f * VU0Regs.I.f);
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.Acc.Y.f + (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.I.f);
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.Acc.Z.f + (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.I.f);
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.Acc.W.f + (VU0Regs.CPR[VU_FS].W.f * VU0Regs.I.f);
					}
					break;

				case 0x0000027C: // "vsubaq"
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.CPR[VU_FS].X.f - VU0Regs.Q.f;
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.CPR[VU_FS].Y.f - VU0Regs.Q.f;
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.CPR[VU_FS].Z.f - VU0Regs.Q.f;
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.CPR[VU_FS].W.f - VU0Regs.Q.f;
					}
					break;

				case 0x0000027D: // "vmsubaq"
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.Acc.X.f - (VU0Regs.CPR[VU_FS].X.f * VU0Regs.Q.f);
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.Acc.Y.f - (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.Q.f);
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.Acc.Z.f - (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.Q.f);
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.Acc.W.f - (VU0Regs.CPR[VU_FS].W.f * VU0Regs.Q.f);
					}
					break;

				case 0x0000027E: // "vsubai"
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.CPR[VU_FS].X.f - VU0Regs.I.f;
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.CPR[VU_FS].Y.f - VU0Regs.I.f;
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.CPR[VU_FS].Z.f - VU0Regs.I.f;
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.CPR[VU_FS].W.f - VU0Regs.I.f;
					}
					break;

				case 0x0000027F: // "vmsubai"
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.Acc.X.f - (VU0Regs.CPR[VU_FS].X.f * VU0Regs.I.f);
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.Acc.Y.f - (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.I.f);
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.Acc.Z.f - (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.I.f);
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.Acc.W.f - (VU0Regs.CPR[VU_FS].W.f * VU0Regs.I.f);
					}
					break;

				case 0x000002BC: // "vadda"
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.CPR[VU_FS].X.f + VU0Regs.CPR[VU_FT].X.f;
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.CPR[VU_FS].Y.f + VU0Regs.CPR[VU_FT].Y.f;
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.CPR[VU_FS].Z.f + VU0Regs.CPR[VU_FT].Z.f;
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.CPR[VU_FS].W.f + VU0Regs.CPR[VU_FT].W.f;
					}
					break;

				case 0x000002BD: // "vmadda"
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.Acc.X.f + (VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].X.f);
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.Acc.Y.f + (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].Y.f);
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.Acc.Z.f + (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].Z.f);
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.Acc.W.f + (VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].W.f);
					}
					break;

				case 0x000002BE: // "vmula"
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].X.f;
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].Y.f;
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].Z.f;
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].W.f;
					}
					break;

				case 0x000002FC: // "vsuba"
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.CPR[VU_FS].X.f - VU0Regs.CPR[VU_FT].X.f;
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.CPR[VU_FS].Y.f - VU0Regs.CPR[VU_FT].Y.f;
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.CPR[VU_FS].Z.f - VU0Regs.CPR[VU_FT].Z.f;
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.CPR[VU_FS].W.f - VU0Regs.CPR[VU_FT].W.f;
					}
					break;

				case 0x000002FD: // "vmsuba"
					if (VU_DEST_X)
					{
						VU0Regs.Acc.X.f = VU0Regs.Acc.X.f - (VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].X.f);
					}
					if (VU_DEST_Y)
					{
						VU0Regs.Acc.Y.f = VU0Regs.Acc.Y.f - (VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].Y.f);
					}
					if (VU_DEST_Z)
					{
						VU0Regs.Acc.Z.f = VU0Regs.Acc.Z.f - (VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].Z.f);
					}
					if (VU_DEST_W)
					{
						VU0Regs.Acc.W.f = VU0Regs.Acc.W.f - (VU0Regs.CPR[VU_FS].W.f * VU0Regs.CPR[VU_FT].W.f);
					}
					break;

				case 0x000002FE: // "vopmula"
					VU0Regs.Acc.X.f = VU0Regs.CPR[VU_FS].Y.f * VU0Regs.CPR[VU_FT].Z.f;
					VU0Regs.Acc.Y.f = VU0Regs.CPR[VU_FS].Z.f * VU0Regs.CPR[VU_FT].X.f;
					VU0Regs.Acc.Z.f = VU0Regs.CPR[VU_FS].X.f * VU0Regs.CPR[VU_FT].Y.f;
					break;

				case 0x000002FF: // "vnop"
					break;

				case 0x0000033C: // "vmove"
					if (VU_DEST_X)
					{
						VU0Regs.CPR[VU_FT].X.f = VU0Regs.CPR[VU_FS].X.f;
					}
					if (VU_DEST_Y)
					{
						VU0Regs.CPR[VU_FT].Y.f = VU0Regs.CPR[VU_FS].Y.f;
					}
					if (VU_DEST_Z)
					{
						VU0Regs.CPR[VU_FT].Z.f = VU0Regs.CPR[VU_FS].Z.f;
					}
					if (VU_DEST_W)
					{
						VU0Regs.CPR[VU_FT].W.f = VU0Regs.CPR[VU_FS].W.f;
					}
					break;

				case 0x0000033D: // "vmr32"
					if (VU_DEST_X)
					{
						VU0Regs.CPR[VU_FT].X.f = VU0Regs.CPR[VU_FS].Y.f;
					}
					if (VU_DEST_Y)
					{
						VU0Regs.CPR[VU_FT].Y.f = VU0Regs.CPR[VU_FS].Z.f;
					}
					if (VU_DEST_Z)
					{
						VU0Regs.CPR[VU_FT].Z.f = VU0Regs.CPR[VU_FS].W.f;
					}
					if (VU_DEST_W)
					{
						VU0Regs.CPR[VU_FT].W.f = VU0Regs.CPR[VU_FS].X.f;
					}
					break;

				case 0x0000037C: // "vlqi"
				case 0x0000037D: // "vsqi"
				case 0x0000037E: // "vlqd"
				case 0x0000037F: // "vsqd"
					break;

				case 0x000003BC: // "vdiv"
					switch (VU_FSF)
					{
					case 0: TempF32 = VU0Regs.CPR[VU_FS].X.f;   break;
					case 1: TempF32 = VU0Regs.CPR[VU_FS].Y.f;   break;
					case 2: TempF32 = VU0Regs.CPR[VU_FS].Z.f;   break;
					case 3: TempF32 = VU0Regs.CPR[VU_FS].W.f;   break;
					}
					switch (VU_FTF)
					{
					case 0: VU0Regs.Q.f = TempF32 / VU0Regs.CPR[VU_FT].X.f;   break;
					case 1: VU0Regs.Q.f = TempF32 / VU0Regs.CPR[VU_FT].Y.f;   break;
					case 2: VU0Regs.Q.f = TempF32 / VU0Regs.CPR[VU_FT].Z.f;   break;
					case 3: VU0Regs.Q.f = TempF32 / VU0Regs.CPR[VU_FT].W.f;   break;
					}
					break;

				case 0x000003BD: // "vsqrt"
					switch (VU_FTF)
					{
					case 0: VU0Regs.Q.f = (float)sqrt(VU0Regs.CPR[VU_FT].X.f); break;
					case 1: VU0Regs.Q.f = (float)sqrt(VU0Regs.CPR[VU_FT].Y.f); break;
					case 2: VU0Regs.Q.f = (float)sqrt(VU0Regs.CPR[VU_FT].Z.f); break;
					case 3: VU0Regs.Q.f = (float)sqrt(VU0Regs.CPR[VU_FT].W.f); break;
					}
					break;

				case 0x000003BE: // "vrsqrt"
					switch (VU_FSF)
					{
					case 0: TempF32 = VU0Regs.CPR[VU_FS].X.f;   break;
					case 1: TempF32 = VU0Regs.CPR[VU_FS].Y.f;   break;
					case 2: TempF32 = VU0Regs.CPR[VU_FS].Z.f;   break;
					case 3: TempF32 = VU0Regs.CPR[VU_FS].W.f;   break;
					}
					switch (VU_FTF)
					{
					case 0: VU0Regs.Q.f = TempF32 / (float)(sqrt(VU0Regs.CPR[VU_FT].X.f));   break;
					case 1: VU0Regs.Q.f = TempF32 / (float)(sqrt(VU0Regs.CPR[VU_FT].Y.f));   break;
					case 2: VU0Regs.Q.f = TempF32 / (float)(sqrt(VU0Regs.CPR[VU_FT].Z.f));   break;
					case 3: VU0Regs.Q.f = TempF32 / (float)(sqrt(VU0Regs.CPR[VU_FT].W.f));   break;
					}
					break;

				case 0x000003BF: // "vwaitq"
					break;

				case 0x000003FC: // "vmtir"
					switch (VU_FSF)
					{
					case 0:
						VU0Regs.CCR[VU_IT].u32 = VU0Regs.CPR[VU_FS].X.u & 0x0000FFFF;
						break;
					case 1:
						VU0Regs.CCR[VU_IT].u32 = VU0Regs.CPR[VU_FS].Y.u & 0x0000FFFF;
						break;
					case 2:
						VU0Regs.CCR[VU_IT].u32 = VU0Regs.CPR[VU_FS].Z.u & 0x0000FFFF;
						break;
					case 3:
						VU0Regs.CCR[VU_IT].u32 = VU0Regs.CPR[VU_FS].W.u & 0x0000FFFF;
						break;
					}
					break;

				case 0x000003FD: // "vmfir"
					if (VU_DEST_X)
					{
						VU0Regs.CPR[VU_FT].X.f = (EMU_I16)VU0Regs.CCR[VU_IS].u16_00_15;
					}
					if (VU_DEST_Y)
					{
						VU0Regs.CPR[VU_FT].Y.f = (EMU_I16)VU0Regs.CCR[VU_IS].u16_00_15;
					}
					if (VU_DEST_Z)
					{
						VU0Regs.CPR[VU_FT].Z.f = (EMU_I16)VU0Regs.CCR[VU_IS].u16_00_15;
					}
					if (VU_DEST_W)
					{
						VU0Regs.CPR[VU_FT].W.f = (EMU_I16)VU0Regs.CCR[VU_IS].u16_00_15;
					}
					break;

				case 0x000003FE: // "vilwr"
				case 0x000003FF: // "viswr"
					break;

				case 0x0000043C: // "vrnext"
					if (VU_DEST_X)
					{
						VU0Regs.R.f = ((float)rand() / (float)RAND_MAX) + 1.0f;
						VU0Regs.R.u = 0x3F800000 | (VU0Regs.R.u & 0x007FFFFF);
						VU0Regs.CPR[VU_FT].X.u = VU0Regs.R.u;
					}
					if (VU_DEST_Y)
					{
						VU0Regs.R.f = ((float)rand() / (float)RAND_MAX) + 1.0f;
						VU0Regs.R.u = 0x3F800000 | (VU0Regs.R.u & 0x007FFFFF);
						VU0Regs.CPR[VU_FT].Y.u = VU0Regs.R.u;
					}
					if (VU_DEST_Z)
					{
						VU0Regs.R.f = ((float)rand() / (float)RAND_MAX) + 1.0f;
						VU0Regs.R.u = 0x3F800000 | (VU0Regs.R.u & 0x007FFFFF);
						VU0Regs.CPR[VU_FT].Z.u = VU0Regs.R.u;
					}
					if (VU_DEST_W)
					{
						VU0Regs.R.f = ((float)rand() / (float)RAND_MAX) + 1.0f;
						VU0Regs.R.u = 0x3F800000 | (VU0Regs.R.u & 0x007FFFFF);
						VU0Regs.CPR[VU_FT].W.u = VU0Regs.R.u;
					}
					break;

				case 0x0000043D: // "vrget"
					if (VU_DEST_X)
					{
						VU0Regs.CPR[VU_FT].X.u = VU0Regs.R.u;
					}
					if (VU_DEST_Y)
					{
						VU0Regs.CPR[VU_FT].Y.u = VU0Regs.R.u;
					}
					if (VU_DEST_Z)
					{
						VU0Regs.CPR[VU_FT].Z.u = VU0Regs.R.u;
					}
					if (VU_DEST_W)
					{
						VU0Regs.CPR[VU_FT].W.u = VU0Regs.R.u;
					}
					break;

				case 0x0000043E: // "vrinit"
					switch (VU_FSF)
					{
					case 0: VU0Regs.R.u = VU0Regs.CPR[VU_FS].X.u; break;
					case 1: VU0Regs.R.u = VU0Regs.CPR[VU_FS].Y.u; break;
					case 2: VU0Regs.R.u = VU0Regs.CPR[VU_FS].Z.u; break;
					case 3: VU0Regs.R.u = VU0Regs.CPR[VU_FS].W.u; break;
					}
					srand(VU0Regs.R.u & 0x007FFFFF);
					break;
				}
			}
		}
		else
		{
			if (OpCode & 0x01000000)
			{
				switch (OpCode & 0x00FF0000)
				{
				case 0x00000000: // "bc2f"
					// Branch if COP2 is not running, always for now
				{
					EXECUTE_BRANCH(PS2Regs.R5900Regs.PC);
				}
				break;

				case 0x00010000: // "bc2t"
					break;

				case 0x00020000: // "bc2fl"
					// Branch if COP2 is not running, always for now
				{
					EXECUTE_BRANCH(PS2Regs.R5900Regs.PC);
				}
				break;

				case 0x00030000: // "bc2tl"
					PS2Regs.R5900Regs.PC += 4;
					break;
				}
			}
			else
			{
				switch (OpCode & 0x03E00001)
				{
				case 0x00200000: // "qmfc2.ni"
				case 0x00200001: // "qmfc2.i"
					PS2Regs.R5900Regs.Reg[R_RT].u32_00_31 = VU0Regs.CPR[VU_FS].X.u;
					PS2Regs.R5900Regs.Reg[R_RT].u32_32_63 = VU0Regs.CPR[VU_FS].Y.u;
					PS2Regs.R5900Regs.Reg[R_RT].u32_64_95 = VU0Regs.CPR[VU_FS].Z.u;
					PS2Regs.R5900Regs.Reg[R_RT].u32_96_127 = VU0Regs.CPR[VU_FS].W.u;
					break;

				case 0x00400000: // "cfc2.ni"
				case 0x00400001: // "cfc2.i"
					PS2Regs.R5900Regs.Reg[R_RT].u32_00_31 = VU0Regs.CCR[VU_ID].u32;
					break;

				case 0x00A00000: // "qmtc2.ni"
				case 0x00A00001: // "qmtc2.i"
					VU0Regs.CPR[VU_FS].X.u = PS2Regs.R5900Regs.Reg[R_RT].u32_00_31;
					VU0Regs.CPR[VU_FS].Y.u = PS2Regs.R5900Regs.Reg[R_RT].u32_32_63;
					VU0Regs.CPR[VU_FS].Z.u = PS2Regs.R5900Regs.Reg[R_RT].u32_64_95;
					VU0Regs.CPR[VU_FS].W.u = PS2Regs.R5900Regs.Reg[R_RT].u32_96_127;
					break;

				case 0x00C00000: // "ctc2.ni"
				case 0x00C00001: // "ctc2.i"
					VU0Regs.CCR[VU_ID].u32 = PS2Regs.R5900Regs.Reg[R_RT].u32_00_31;
					break;
				}
			}
		}

		///////////////////////////////////
		// End of COP2 Functions
		///////////////////////////////////
	}
}