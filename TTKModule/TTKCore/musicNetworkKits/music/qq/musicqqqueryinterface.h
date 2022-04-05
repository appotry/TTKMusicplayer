#ifndef MUSICQQQUERYINTERFACE_H
#define MUSICQQQUERYINTERFACE_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2022 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include "musicglobaldefine.h"

///cookie url
const QString QQ_UA_URL                 = "NW9YVlRXYnBnNlBFS3Ardi9VR295R0l2aW43U3JvRlpWc1AxMkI2K1MvN3RMVjdsOGhZMVlPRmIrais2ajBJUW1UbmxBRjlyS25WNHRyU0FYbFFMeWFpMUo5YWxFNnkweHd6RERnPT0=";
const QString QQ_COOKIE_URL             = "WlR0WU9LOHhVWXlHZ09paFJhem5HOHJHeW1ZaGVGRWFKd0NoMWRZUFB2WlI2ZGxIZy8zdjMxU3RqL1lvcVB2V3VMeTlMd2o2MXR0SmZBZGhHNFF3UGhEMEhzUllNVmJxZ3VqRTdFUlQrKzFpZjc3eHkrcjNrbFlXa2QrQ2FoZ2hhKyt0clJWaG1jWT0=";
const QString QQ_BASE_URL               = "RVNEYVc1Z1l4Wk9Ma2QxMEl3bnNnNEJaS0hiZVlZNnBQZXlQTUIzZGFjMksyVkNPMkxaeXBnPT0=";
///song url
const QString QQ_SONG_SEARCH_URL        = "SEVLZy9PdHJyRFFVWEw4M2R1b0hGVkxuZWFVZTFDVnhjZmFsZlk0ZFpDWENMK2FKM00xa1V5T1Vsd3gvcWxYcStqSHZTdm5lK0dvazkybWc1QklsMWx6ZlhtdkpsRFpJSVF4cWNhRVpDSG5GRnBFWmN2T3B2SXp1eCsreXZVVFNXVHluQVlZTEdaK01LcjJOWTV1TVdvTnRaVFZjVldTcUxHYTk1dCs1Y05MaCtFa1QrQnJ0ekJSNE95TFZNV3BaVXVZVUhTVTJpSjJPK1VVK2E5NXEzeWZrYXhucEMwZkh3bVBad1FaOHJZTmZGdEs0aEkrcFptSURBNTRIZExZYlVCbDRJWHgreFVuL1FDUUgvdmdBeVFTV3BabDhiYStrajE2TkN4ZFJYaWNoY3NpMVI0OGtYcEJ2NVcybkFTN29WZnNNVU5EOW9kSVlrNllMM3BLMkhsS2pZRCtLUitUbVJTa1JkeVZ6ZWpaRzRoYTlYVCtBOFJDSU0zTm55WEtGVWNSNTBPMXdJMFE9";
const QString QQ_SONG_INFO_URL          = "VTl3Ykp1cUk5TEFvY1JIc1E2aUFneXV4Y1VGRlVxVFM5ZTI2a283bExrNzk3WFl6ZEdneVdVN0JDeC9jUFRwVG9lTDBCVVphNWNlQTVXcnovV0lSZnZFOEZQcDFMdnNOdW1naitVdFJ4MFU9";
const QString QQ_SONG_LRC_URL           = "UkRvUTU1MHJGMmVDNDhRUWpXbnluVzViaVo0dGVKUHlxWHpnYzEvMnR2VU81WlJxYWpqY0ZEMC84anhRN3kwbm0vUGNxR20zZ1laUzg4Um5EOEdIL0FGS3pvK3VUMEgwMW1BaHFSYlA1OHJUSC9tQmpjUWVxTXk3RFo4PQ==";
const QString QQ_SONG_PIC_URL           = "ZUZWTnkreFptcVRkQ3Z3YW9hdVRHUi90Q2hLWjlrLzB6NlFDSUNnZWZKNnZhUXNXWCtpS3hoZGVpZGlFbmdwRDhWNmFxb3JMTUIyYlY2NDQ=";
const QString QQ_SONG_KEY_URL           = "Y3hObndiQ210ZTl2dCtuU0h3bXk5WDhRYm94TTFuaFJkSWpxcGN5cXNPTy85WDBqU0V4aG5raCtXK3pXdk9ISHBQRE8vVnduREdHZzgwS0hCWFY2d0ZWMTIwRnR0TjU0blFwYmx6TjlSZXBuNjVTVEowc3psbDliVnhtUDJQZm03dDg0c0YyZmtySXJZZ0s4TnN5bkxQcGlmazJqRU5aeTFXL0xIczlpNFgyWlRLVU94ekd4OU9YL0hxeFVKeXZRLzIrSndLTkpxbUtsYUtaY3R0TjJObHJMUTN2Qkc3Z3paOFA0emR0Uy9PMTF4L3d1TUxyTmxvSmI1N2xrZEZVbTdRM0VWUWNaMHdiK1BFU2FpTnYvRVFNbFZ5THhtQVIxM1VHLy90TkhoRWtVc3h5SnhGUCtUVXc5RzdNZG5lTmo4N1ZrWGJqcjVQbEV1VEc2MGgwTmVwajhKcDBMZWNzamljSjU5clJDSWZ4b1lrYWJLbXluSVdkQ1VJUXNRWjlqTjVudWJOblIyMWgyRG9NaER1alhMMUZtY3pONDlTQjhkaFpjeGM2ZmRVK2Q1Mk9GN1FCaFY1QW5UQUlScW1zWUY3WHNaNTUxMEdKTkRYdWVCbzRTd2lLdEJrNTdOZW5Vd2hyZ3JML0V2dlc0b2ErbDVqTzZtRVBtSUkxcDNyYzBIYkNoOWtzNmVzZ3VvbGZvTGlZcld2K2hkVmc9";
///artist url
const QString QQ_ARTIST_URL             = "WERjRFh4WVhsWTdFOStMNWZTSnJGQjlTZHk5RVBqaTUyclRFRjZWVng5bExXNC9GTGdrRFFGQmxTbHpuMmZlSmZacTNLd0VBY21kVDdva1Q0aXlmcU9OWVMwNWtmbFZEbDVRa0JvZCtTWFBLVUtxUjNadmJweVQ2TVF3L0I5WlQ2Z05WcmNPMGZoWjh6d0ZUWHBmTjNKSnhGanVIcEt1L25IS0ZncDU5SzBLZ0o1ZzEvTUludEp4Y2NDbzJWc3VPS21kenpadTVhWDRldkwwUkgxMldSYkpBdC9PUWUvcUVJemZvMzFSVDRKNVBUSzhReUZMYXZWK1BxOG9PYTdWa1JSamxlTnQzNzVtcVJsRUdkRE1ONVhMdGRJQT0=";
const QString QQ_ARTIST_INFO_URL        = "UjR2VWxJSlVDeWVJb3FzY0N5VTBBTUZXeGQ1N3l6ZXB3eU92anVaQW1mRmtTUXE5MkFiZzhjQWZJM2JScmxwSm5FRlA1dUpVUDVEaG9LU3poYmppelgxbFU1OXE1N0N0M2JNWlZKeDdUZCswSXFoYkhYSGNVZURuUldOMExzRmZDUlBROHl0OTFBUnBQc0UxK3RkNnR3PT0=";
const QString QQ_ARTIST_ALBUM_URL       = "REp6M2RPcGhXRE4xb3JCVlQ2VWZRdG9tUnZpbnQxR3N6cDhDaG5LTkZnZHJ2TG5BTEcrVW9JV3B4RmVlUjJMVGlsSUd5cmZHRVRxYTgxK1A5ckV5MElDWXJ1RmFKbVR5YkkyeTltWk5PUTJ1b0tMUW92cVdEcGJBZ2NGajdqZlN3djBsa3NIUFRaOFpCckIxeC9XaGZzSXE3cXlKSG1BUGRwNVAyK1VVMUVnYTZjTGE2MlRRMDVEcVJzZjhxL0lkaEJsQjFtQi9mYjYvT0hHSkRCYnBhMitnaFBqZHJRdm1KOGNBZHo4T3B6eWU4NTRPamM5YVRiRlBWRzg9";
const QString QQ_ARTIST_MOVIE_URL       = "NURjT21UVW9mVmRObnllV2RZNGZVUjB1RDhpOEt5cVE5U2x0L2JrcFl3ODF1TlY2NVVYTEliVzNTZ0U1TitYcW9jcjNJUDU0M1BYMzlUMWtxOTlUWU9NR295U255blB3U3BpTGJlY3NvVDEybmx1TWJrN09tZzlnK1F0WUZQTFREcVM1WDRNQ3pMWW5KK0p2U3dWOXlFNFpxWHNZbFZ1L2FIeHExVWpCTXUrYldwaXBna1c1RGlmU24xb2pFQUxNY0xoeVlETzZXZTV0cTZTV01UazhVc1NVMmcvTUlONEtYZ3pzYVFCZUdBS0kwc3dvL1NnSmRTM2JoVVlsS0xJa2s0endNdz09";
const QString QQ_ARTIST_SIMILAR_URL     = "Q3l2R1ZmOGdsWHRIWTVtZGZxRXZUZ3F0VllzZDlTc0M2TmNkQ0N3aHVNZHVQbG1kb0gwS0JGbGRQN3lWblcrUnYydFFpWUFZTHlaVHcxMmdzbWdvNmRMQVVRV0RYSVZBT3dFWS9qMm5zS2NFdEFZMXJuOXVzQU5SQ1ErWU1BWlFMVGIrOFdTcXcySXlMUmh4UEttTDlpY0tQdFBOc2NLbUlmTjNDQ1ZIOG9yS0dYdGhsMWFWSkhqQkRUcVZlTE5VS3prVEhEYUtoOFdqYUM1dmxRU1RoVExwL3pHRDdyb09SdG5rdkw3YjdTVlpFMEQzRG1BN1BiYmNaVTliWVM5bjg2Mm8xVG1UcU9TUjlGSGQ4MWkvWEE9PQ==";
const QString QQ_ARTIST_LIST_URL        = "UGx5NDFTQzJ2V2Fpb3JxTnVHVVBmM0p1SlJ0RTg0ZGp4LzBxbzBpVjB3bWM2ckZSMytkVGdQSVZZNGRaU0M3YXFkWWxkYUxPS015VWdpQ2RVOVZRMTFPVERJZ0NXOEtHV0p1V2I0TTh5YlBab1FYcGV6eXBFa0hoQmxZZzQ3N1BBUkoveWJraWdYWDVLOVdnOWp0OVMzYityVnNwaFh0d2VlcVI1dUkwUFI4UTFGdjBpZjhoZFBWVnUzWUlyWk1Qc0praUVtSUVXelpodGkrRlVsVENSWFl6R0V2eXZqeDRrTWNSQWFwTFZucnlYeEJsNkpDeTg3d0tKUzFVcW9RY2ZtT2NuWUNMby9HSEc3b1hubDRIdk05WTlGYz0=";
///album url
const QString QQ_ALBUM_URL              = "THpHd1YyR3FTL3RCQ2pLMFVwSTlrTEc1WUszYXBKNWFqTHRSMXMrSzJIdUJURWx3REoyOWZ0dmRxRXpmSGdaTlRGMTltYmdmZlU4RkFMbWRuTVhtZ0VBWmRVM3A1eVgxVVgzcUZwYkloQzdSNWJSL3VjTGZTNTRBbWJWekRxZ1NXYkdBR1NocW40cm9wRVYrU1EweVN1MHZFeVliM09zUmQ5eHI1RzBUZjU4by9MZVAyZnh1dXR1QkJhdE8zQnk5SlFCcm1MZHVabTRSTkFJSGhmNEgyQVlCc1o3UjR0VGQ=";
///movie url
const QString QQ_MOVIE_INFO_URL         = "ZjhkZmd4LzNnQi9seFJpUmZ0L0hLQkZ1dFdnME5lOWI5dlgvUFpHUUw5U2xQdzJXZzhxditTWTNTWllKTVp4dFo2bmlqc1Y4TUpoeTFEdUFnbkNVOE5WL0RVYVFzSHR0UFpzN2pBPT0=";
const QString QQ_MOVIE_KEY_URL          = "cEo4MkdYaVNIUEJFTGNnd0hGai8zUFVWQStTQ3NyamM1YmovTzdGQWRwRFU2OExQV0MxMUxSN0NkSmJrT3MzTTNVaFhSUytNWmFrKys3ODdhWldaNWljZXBaRmFHeUg1dHdGb1JSNEJpQ3orZFIwVUIrUURJUFRJZ25SakVpN20=";
///toplist url
const QString QQ_TOPLIST_URL            = "NGtsYzRNNjFCSlhOVUFnMk0rSHFpZll4WUdsY0ZWcERWWXFtbnBIU2JGV0lWLzhNU2NFWUpaS3ZaOTZFdm9VQlBLdzl0S0FOTEhaNDhpQmkzSGt2QXFLcnhDNjNuZHNQOVM2Tk1UMjRReHVFRnQ3eS9JakUwOHBYa2RXUzNtcGQwaTNEdUJEZXlYdllya2xPWXVzRkFqRFFWV29nSkowN2NuNmxxRkhTaFFRZzhCZSs2bXIzK1hzNHhoakppdGM5TTVFNlkwV2pRV3hpbzR4WTFNdU55STZ2UVdnVVRVRlBZMjA2U1pzbVdobWd6S3lEMGZtVWJpbUhwT011U1pJRw==";
///comment url
const QString QQ_COMMENT_SONG_URL       = "eEFLWHlGK0pkckx6bE5mTnR1T2l2am1uaGxYdUlsWHZTRlgwN3JHUC9sU1hMTzVvR1I3SHB5NVpSZEJ1QklDOGlGa2djci9IRkRseElHL0hGUTJrUThNcU95LzlVWVU2eVdRYVhlUzZ6NTlYcFIzZ2R1YjJUTFQwTngvbWlKRWdobHFYYWpJM2IybmpBVVpqWTBRRFY4dzZZeEpPSS8yRUxHTDdZdVFQQVpNaHRDN3hwWXlpdC9RYmd3aTJSVTdZSTI3MDlMRFZWbkJQOWhxSTJpTEVwYU5kQ0RMaFR6RmgrbVRnOUp2NFJOekczczI5bGpZWUpIRjN4T3BMYTJGMXF0RllsUnZjdHdYMEpMNUdYOFJDdHF6djh2TSt6cEdrSWN0SmVQQXA4MGxHNDRkeldaY01KWUIrUTl1Ky9Lczg3NEpWOXptV0ZocTlaaVZudmZVYWd5MkFDbWtYa2V0OXFrSW05emErdWFzK2gyVGlFcEdsdHBlVXJmQVBRejk1OFFzYW92cThEUm5pUUMyK0VjMWU2a0cxRnF6bjVRZ0p4bnpHYkNOUFNvZHZZaGF5SEpzaDFyQjZwbmlKTUVxc3Vya09MdGphK1Z6eGpoYlJxTWVzMTlkT0o4Y2daeFlmYktFczRRalptb21HSGdBeldNcmdvejRjM3BsdkxjTmFKdFhLbDNKZ0NJVktxUFpuZExYR0ZoRjZYZTJ5VXdScTl5eVpnVEhKSzB6cGJadS9KNUtlVEhhRGxLRGdYdzZyWHdDV3RoWjlMNEVES2NJbDhJVXc3T1BTUVlLS3RsVjJaa3V2SEVwYnhwdVFMOUQ2bXFSTGtpbm9GWGNUWWlXbUhrNUt0aHNnMGd1WEpCT2FKYnQyUmt5L3ltVWUyNHpPbTB1ekR3aEJjcGt4cW1oaGdiYVJGdz09";
const QString QQ_COMMENT_PLAYLIST_URL   = "OFVHemxSOFdnZTJTSkVXNU9nR1U3MTJ4OVRtTGhXRzE2THVTNnBoMWpoMGpvK1pUUVhtYXM5eHJvTUFKSksraFBhbzJLZldic0N0akxqc0RadW1UdjFka2M2aDdxZG5KYktKeklQZjB0M0x6Z3VDSjZnNEtGYWNPWDFtNERTOU9GNVdkUTdPWDd4b29wUm5ZZ2NGSWxqTFJqOE43TmpSNm83K1BENnRER0MzLzJSNDZXdlE1a1o5dHQrdE1NUkNYbUxjblEyMU1EUFo0NE1PWFBaRmloOW96bGxROFF3NW5qUFZsVzg4RE14K3psUkpsNHc4T2U4dVFOVlo4eDlqZm5SZm9pWHFaQXUvbThVMDlxSGp6UjNpMjVqZTBUR2M3QzJNWGhaclNZdjBWRjF2NC9oSy84b3dSK1B5RVJCdlZXaytqYStPNFprWmpKQ2xvaXJ6SWY4SUpsd2xvT29LenRQbmdMaVdIRkNld2MyMHozUXYwUllJUi9XQmV1MmJBNHAzME9rU25FbDg5YW5TYXV0MlpPeWltQWtUTzFVdVBzZkhQKytaY1B0Ti8zZ0VFaXEzUmRqY2FHUW1PN2hYZTM3UWozbEJKR09vaEk1V3paWnJCcHNVKzQ5ZUt2VUkvclJLeDdlU3dKZEc1Tk5PZGg1TlNlQWR0UnNYZlRIamlYemc5Ny9HR0E5M0tFSmgzUVlSODFJcDQ0c1J2dXZ6bFd5UVU4SHc0SHVtY01zZjFkcW5vTHhza1BKZGwvVVlOZm80ZmpsOWY0ZDcxTWZSVHE5MVoxNmVYdEl2ZG5SQWtBUjF5STE3NnFJS3NrVkFQRjNTWjM1Q08xMEJablYzcmpTV0o2K29kQ3ViWWRkc0ZmTDFScTFjTjdENjZuTy84cWtFYmU2S2FrMFBmZjV3bm9SckZndz09";
///playlist url
const QString QQ_PLAYLIST_URL           = "M2xMcmljcUNoVDNKemw1YWhEWkIxUStMUUNySnYwdUxHUlQvRTcyanBPRnR6TEU2ZlBUZXc3QllEd25lSjBKT2hMeGYrQnNZMmhiSmZKd3UrWjJISWNJWlJ2MFpFTm95VjB2REdLM2RPUVlpeWhGblUvZ0toOWJFTUlKak81ZElybERrTk5JSHJBUXMyZjJraTcyejZVUE1FbDJtMUhwU3VyWFl4Tk52QVM4QnliWFlERzEzdGw1aFBSb0lRVy9NVThlT2VOOFY3MlV5RDFtcVBRelFhbjNzRUdqQWtPbVJKMllWWGdOZFROKzZPcnc1bCtySnd1R0hINVdzemJWR3R2NEd6OFhOQlhVME14eEI3WVlXTFpzZGVuUDZLR2Nw";
const QString QQ_PLAYLIST_INFO_URL      = "OVNWZXI3NkNOVXVONnZMWXNMaitKR3NzcmJKcFNKTEpaNTBQT3RyTzM3OWJ2Y2c4dTFlOHpHcDh0c1p3OFpFWEJySUtUQjRUUDFVU0YyRGdjR3RGZmt2YlVTSFMyaEE1TWtKRDVjcy9qZXNWL3hKbFR4SDljK05PVjY0WXZRdUVHNFZhRkNpZ2EvNGRaWmpJWFhRM0JPS3pVa0w4Tjh4NUpFd0M5Z0dtVGNMNEMrZzUzTkxZeDRDNDlYM01BVkpSWFdjUUkzS25UMVczZUF1MnZjak80Qk15QkRDYzkrM2lUTEoyeG1HSmlMUFNxckF0dmh1bTNUcDhhZ1Z4WTdKTXRVOFhINDBZYm9XUDUzVzdjQXpuTldoeTJmVFZDTGh0UHo0Q3lJSHZOb1A4VnkyQUxGQnNpdFZsd1gzQ0lSQ0Q=";
///recommend url
const QString QQ_RECOMMEND_URL          = "dEFUcDF6NjZKOVZ1bk01OTFhVHZBd3Jlb1J2K08xVkZsVkw4R054SGp3eXluSS9pREZrMGUwUEo1WnJCbHVla0tjdHAvajViN3puMzUzNVJFdXBoTzArbHl4amZObXhCcnczUTh6TXZxMFBvTWFUQXlQTy9hMHVacWJvUUE5NGpvbmVXd3VKMGJCcHVkRzk5bmZ1N0tPL2RCUjNRZGFMNlU4MGlOYmJaSEpaRWxDVmkzNFVoSklPVHg1dXByVDZNanArZkpzMVovY2JBM0IxN0RoMWlwUWVQakxiZlQvMEt4b2JNWGZyQldoQUs5MUsxUTY3eDk4UCtFRm9PaTA1TkpkSXVzUWx6dUJsaTBCV3FMc2g2djNNRkZDVEltZmNVTzN5ZTg4cUZJZmpnUm13RklGTnJHNEhTSHAvdmVmWUdaYVJBK0JwVlM5Rlk5Sm13aXd0NjlrZHFqU0V2NTlEa3VWWnVudUZYWFJ5cWMveVdDQ1BJeDY3bEJpczZvekJ6OVFEMXJtQmRxUFk9";
///suggest url
const QString QQ_SUGGEST_URL            = "YVZrME9hWGRKNjJySkE1djh3T0wwVG1TY1FzVU94ODY2aW83TEFqRVROWVRpU2EzcG00NGxvZ0RYcm8xdE50bTkvSHdKMjc3aW9kbENxbnpRMnU4RzVtRVJDZW4xMENuRytnVDhWMEV3WUtrME12VFlxNTRKdGtaMGhSVDJHRmdONVY5aU9uOXBkSDdqK1drSEZnYW9tZi96NHVOc0NLQWVXRi9iV0t0cnNGckVDbjF6WVRaWmNJN1B4R0hpOVM5MUZxODFjTk16N3pEem95UVphNW81a3hSY1RjNXZzNzVzVzBvZjRkZGlHUjcvQTBOZEdKSDFnPT0=";


class QNetworkRequest;

namespace MusicQQInterface
{
    /*!
     * Make request query data.
     */
    void makeRequestRawHeader(QNetworkRequest *request);

}

/*! @brief The class of qq query interface.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicQQQueryInterface
{
public:
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongProperty(MusicObject::MusicSongInformation *info, const QVariantMap &key, int bitrate) const;
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongProperty(MusicObject::MusicSongInformation *info, const QVariantMap &key, const QString &quality, bool all) const;
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongPropertyNew(MusicObject::MusicSongInformation *info, const QVariantMap &key, int bitrate) const;
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    void readFromMusicSongPropertyNew(MusicObject::MusicSongInformation *info, const QVariantMap &key) const;

    /*!
     * Get music key.
     */
    QString generateMusicPath(const QString &file, const QString &mid) const;

};

#endif // MUSICQQQUERYINTERFACE_H
