import { TestBed } from '@angular/core/testing';

import { DisplayBusinessPhotosService } from './display-business-photos.service';

describe('DisplayBusinessPhotosService', () => {
  let service: DisplayBusinessPhotosService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(DisplayBusinessPhotosService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
